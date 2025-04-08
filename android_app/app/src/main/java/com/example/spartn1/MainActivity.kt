package com.example.spartn1

import android.content.pm.ActivityInfo
import android.graphics.BitmapFactory
import android.net.ConnectivityManager
import android.net.LinkProperties
import android.net.Network
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.Canvas
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.ImageBitmap
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.graphics.drawscope.translate
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.input.KeyboardType
import androidx.compose.ui.text.input.TextFieldValue
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.IntOffset
import androidx.compose.ui.unit.IntSize
import com.example.spartn1.ui.theme.SPARTN1Theme
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.Serializable
import java.net.*

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // Force landscape orientation
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE

        setContent {
            SPARTN1Theme {
                MySplitScreen()
            }
        }
    }
}

@Composable
fun MySplitScreen() {
    var showBlueScreen by remember { mutableStateOf(false) }
    // Mutable state for mic indexes received from the Pi (default placeholder)
    var micIndexes by remember { mutableStateOf("Not configured") }
    val classificationText = remember { mutableStateOf("No Class.") }

    // Tower coordinates for each mic.
    // We assume the order received from the Pi will dictate that:
    // first coordinate corresponds to mic index from first position, etc.
    val x1 = remember { mutableStateOf(TextFieldValue("0")) }
    val y1 = remember { mutableStateOf(TextFieldValue("0")) }
    val x2 = remember { mutableStateOf(TextFieldValue("0")) }
    val y2 = remember { mutableStateOf(TextFieldValue("0")) }
    val x3 = remember { mutableStateOf(TextFieldValue("0")) }
    val y3 = remember { mutableStateOf(TextFieldValue("0")) }

    val userCoords = listOf(
        x1.value.text to y1.value.text,
        x2.value.text to y2.value.text,
        x3.value.text to y3.value.text
    ).map { (xs, ys) ->
        (xs.toFloatOrNull() ?: 0f) to (ys.toFloatOrNull() ?: 0f)
    }

    // Live predicted coordinate from the Pi
    val liveCoord = remember { mutableStateOf(0f to 0f) }

    // Live estimated quadrant from the Pi
    val liveQuadrant = remember { mutableStateOf(-1)}   // default -1, means no quadrant

    // State for each tower’s mic connection (ALSA connection status).
    val tower2Connected = remember { mutableStateOf(false) }
    val tower3Connected = remember { mutableStateOf(false) }
    val tower4Connected = remember { mutableStateOf(false) }

    // Destination Pi IP – used when sending location updates.
    val destIp = remember { mutableStateOf(TextFieldValue("10.4.168.69")) }

    // Local IP and port for receiving live coordinate updates and ALSA connection messages.
    val context = LocalContext.current
    val localIp = remember { findLocalIP(context) }
    val serverPort = 39439

    val coroutineScope = rememberCoroutineScope()

    // Start the TCP server that listens for live (x,y) updates and ALSA connection commands.
    LaunchedEffect(Unit) {
        coroutineScope.launch(Dispatchers.IO) {
            startTcpServerForever(serverPort,
                OnClassifcation = { classification ->
                    classificationText.value = classification
                },
                onCoord = { x, y ->
                    liveCoord.value = x to y
                },
                onMicConnected = { micIndex ->
                    // When the Pi sends an ALSA mic connection command,
                    // update the corresponding switch state.
                    when (micIndex) {
                        1 -> tower2Connected.value = true
                        2 -> tower3Connected.value = true
                        3 -> tower4Connected.value = true
                        else -> { /* ignore unknown mic indices */ }
                    }
                },
                onQuadrant = { quadrant ->
                    liveQuadrant.value = quadrant
                }
            )
        }
    }

    if (!showBlueScreen) {
        // GREEN SCREEN: display current tower coordinates and live location.
        Scaffold(modifier = Modifier.fillMaxSize()) { paddingValues ->
            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(paddingValues),
                verticalArrangement = Arrangement.SpaceBetween
            ) {
                Row(
                    modifier = Modifier
                        .fillMaxWidth()
                        .weight(1f)
                ) {
                    // Left Pane: tower configuration and mic indexes
                    Column(
                        modifier = Modifier
                            .weight(1f)
                            .fillMaxHeight()
                            .padding(0.dp),
                        verticalArrangement = Arrangement.Top,
                        horizontalAlignment = Alignment.CenterHorizontally
                    ) {
                        Button(
                            onClick = { showBlueScreen = true },
                            modifier = Modifier.padding(bottom = 0.dp)
                        ) {
                            Text("Switch to Config and Spectrum")
                        }
                        Text("Tower Coordinates (Mic indexes below):", style = MaterialTheme.typography.subtitle1)
                        // Display handshake status (simple SETUP handshake indicator)
                        Text("Setup Handshake: ${if (micIndexes == "Not configured") "Pending" else "Complete"}",
                            style = MaterialTheme.typography.subtitle1)
                        // Display the mic indexes as received from the Pi.
                        Text("Mic Indexes: $micIndexes", style = MaterialTheme.typography.subtitle1)
                        Spacer(modifier = Modifier.height(4.dp))
                        // For each tower, show a row with coordinates, a read-only on/off switch, and a send location button.
                    TowerConfigRow(
                            label = "Tower 2 (from config)",
                            xState = x1,
                            yState = y1,
                            micConnected = tower2Connected.value,
                            onSendLocation = {
                                    coroutineScope.launch(Dispatchers.IO) {
                                            sendLocationForTower(2, x1.value.text, y1.value.text, destIp.value.text)
                                        }
                                }
                                )
                    Spacer(modifier = Modifier.height(4.dp))
                    TowerConfigRow(
                            label = "Tower 3 (from config)",
                            xState = x2,
                            yState = y2,
                            micConnected = tower3Connected.value,
                            onSendLocation = {
                                    coroutineScope.launch(Dispatchers.IO) {
                                            sendLocationForTower(3, x2.value.text, y2.value.text, destIp.value.text)
                                        }
                                }
                                )
                    Spacer(modifier = Modifier.height(4.dp))
                    TowerConfigRow(
                            label = "Tower 4 (from config)",
                            xState = x3,
                            yState = y3,
                            micConnected = tower4Connected.value,
                            onSendLocation = {
                                    coroutineScope.launch(Dispatchers.IO) {
                                            sendLocationForTower(4, x3.value.text, y3.value.text, destIp.value.text)
                                        }
                                }
                                )
                    }
                    // Right Pane: tower plane with live predicted position.
                    Box(
                        modifier = Modifier
                            .weight(1f)
                            .fillMaxHeight()
                    ) {
                        Box(
                                modifier = Modifier
                                            .align(Alignment.TopCenter)
                                            .padding(8.dp)
                                            .background(color = Color.LightGray, shape = androidx.compose.foundation.shape.RoundedCornerShape(8.dp))
                                .padding(horizontal = 0.dp, vertical = 0.dp)
                        ) {
                            Text(text = classificationText.value, style = MaterialTheme.typography.subtitle1)
                        }

                        // Highlight the estimated quadrant that the drone is in
                        Box(
                            modifier = Modifier
                                .fillMaxSize()
                                .then(
                                    when (currentQuadrant.value) {
                                        0 -> Modifier
                                            .align(Alignment.TopEnd)  // Top right quadrant
                                            .background(Color.Red.copy(alpha = 0.5f))  // Highlight in red
                                        1 -> Modifier
                                            .align(Alignment.TopStart)  // Top left quadrant
                                            .background(Color.Red.copy(alpha = 0.5f))  // Highlight in red
                                        2 -> Modifier
                                            .align(Alignment.BottomStart)  // Bottom left quadrant
                                            .background(Color.Red.copy(alpha = 0.5f))  // Highlight in red
                                        3 -> Modifier
                                            .align(Alignment.BottomEnd)  // Bottom right quadrant
                                            .background(Color.Red.copy(alpha = 0.5f))  // Highlight in red
                                        else -> Modifier // No quadrant highlighted
                                    }
                                )
                                .padding(16.dp)  // Optional: Adjust padding for better visual experience
                        )

                        TowerPlane(
                            coords = listOf(
                                (x1.value.text.toFloatOrNull() ?: 0f) to (y1.value.text.toFloatOrNull() ?: 0f),
                                (x2.value.text.toFloatOrNull() ?: 0f) to (y2.value.text.toFloatOrNull() ?: 0f),
                                (x3.value.text.toFloatOrNull() ?: 0f) to (y3.value.text.toFloatOrNull() ?: 0f)
                            ),
                            liveCoord = liveCoord.value
                        )
                    }
                }
                // Bottom bar: shows the listening info and classification.
                Row(
                    modifier = Modifier
                        .fillMaxWidth()
                        .background(Color.LightGray)
                        .padding(horizontal = 8.dp, vertical = 4.dp),
                    horizontalArrangement = Arrangement.SpaceBetween
                ) {
                    Text(text = "Listening on $localIp:$serverPort", style = MaterialTheme.typography.subtitle1)
                    Text(text = classificationText.value, style = MaterialTheme.typography.subtitle1)
                }
            }
        }
    } else {
        // BLUE SCREEN: used for tower configuration.
        BlueScreen(
            onSwitchScreen = { showBlueScreen = false },
            destIpState = remember { mutableStateOf(TextFieldValue("10.4.168.69")) }
        )
    }
}

/**
 * A composable row that displays a coordinate row with an adjacent on/off switch (read-only)
 * and a Send Location button.
 */
@Composable
fun TowerConfigRow(
    label: String,
    xState: MutableState<TextFieldValue>,
    yState: MutableState<TextFieldValue>,
    micConnected: Boolean,
    onSendLocation: () -> Unit
) {
    Column(modifier = Modifier.fillMaxWidth()) {
        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.SpaceBetween,
            verticalAlignment = Alignment.CenterVertically
        ) {
            Text(text = label, style = MaterialTheme.typography.subtitle1)
            Row(verticalAlignment = Alignment.CenterVertically) {
                Text("Mic Connected")
                Switch(
                    checked = micConnected,
                    onCheckedChange = { /* not user controlled */ },
                    enabled = false
                )
            }
        }
        Spacer(modifier = Modifier.height(4.dp))
        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.SpaceBetween,
            verticalAlignment = Alignment.CenterVertically
        ) {
            CoordinateRow(label = "", xState = xState, yState = yState)
            Button(
                onClick = onSendLocation,
                modifier = Modifier.padding(start = 0.dp)
            ) {
                Text("Send Location")
            }
        }
    }
}


/**
 * Row for (X, Y) text fields.
 */
@Composable
fun CoordinateRow(
    label: String,
    xState: MutableState<TextFieldValue>,
    yState: MutableState<TextFieldValue>
) {
    Column(horizontalAlignment = Alignment.CenterHorizontally) {
        Text(text = label, style = MaterialTheme.typography.subtitle1)
        Row(horizontalArrangement = Arrangement.spacedBy(4.dp)) {
            TextField(
                value = xState.value,
                onValueChange = { xState.value = it },
                label = { Text("X") },
                keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number),
                modifier = Modifier.width(40.dp)
            )
            TextField(
                value = yState.value,
                onValueChange = { yState.value = it },
                label = { Text("Y") },
                keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number),
                modifier = Modifier.width(40.dp)
            )
        }
    }
}

/**
 * Blue screen composable for tower configuration.
 * Includes a new "Configure Towers" button that connects to the Pi on port 39440.
 * It receives the mic indexes and then sends the tower coordinates (one for each mic) using the format "micIndex,x,y".
 */
@Composable
fun BlueScreen(
    onSwitchScreen: () -> Unit,
    destIpState: MutableState<TextFieldValue>
) {
    val coroutineScope = rememberCoroutineScope()
    val context = LocalContext.current

    Column(
        modifier = Modifier
            .fillMaxSize()
            .background(Color.Blue)
            .padding(0.dp),
        verticalArrangement = Arrangement.Top,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Button(
            onClick = { onSwitchScreen() },
            modifier = Modifier.fillMaxWidth()
        ) {
            Text("Switch to Green Screen")
        }
        Spacer(modifier = Modifier.height(16.dp))
        TextField(
            value = destIpState.value,
            onValueChange = { destIpState.value = it },
            label = { Text("Pi IP for Config (port 39440)") },
            modifier = Modifier.fillMaxWidth()
        )
        Spacer(modifier = Modifier.height(16.dp))
        Button(
            onClick = {
                coroutineScope.launch(Dispatchers.IO) {
                        // Get local IP for config handshake.
                        val localIp = findLocalIP(context)
                        // Connect to the Pi's configuration server on port 39440.
                        val socket = Socket(destIpState.value.text, 39440)
                        val outStream = socket.getOutputStream()
                        val msg = "configHandshake,$localIp\n"
                        outStream.write(msg.toByteArray())
                        // Optionally, read the acknowledgement.
                        val reader = BufferedReader(InputStreamReader(socket.getInputStream()))
                        val ack = reader.readLine()
                        println("Config Handshake Ack: $ack")
                        socket.close()
                }
            },
            modifier = Modifier.fillMaxWidth()
        ) {
            Text("Configure Towers")
        }
    }
}

/**
 * Draws the tower plane. Displays the towers using the provided coordinates and a red dot for liveCoord.
 */
@Composable
fun TowerPlane(coords: List<Pair<Float, Float>>, liveCoord: Pair<Float, Float>) {
    val scaleFactor = 10f  // This could be made dynamic based on the input extents.
    val context = LocalContext.current
    val towerBitmap: ImageBitmap = remember {
        val bmp = BitmapFactory.decodeResource(context.resources, R.drawable.tower)
        bmp.asImageBitmap()
    }
    val towerW = 25
    val towerH = 25
    val halfW = towerW / 2
    val halfH = towerH / 2

    Canvas(modifier = Modifier.fillMaxSize()) {
        drawRect(color = Color.Green, size = size)
        val w = size.width
        val h = size.height

        translate(left = w / 2f, top = h / 2f) {
            // Draw axes
            drawLine(
                color = Color.Black,
                start = Offset(-w/2f, 0f),
                end = Offset(w/2f, 0f),
                strokeWidth = 3f
            )
            drawLine(
                color = Color.Black,
                start = Offset(0f, -h/2f),
                end = Offset(0f, h/2f),
                strokeWidth = 3f
            )

            // Draw towers
//            coords.forEach { (xVal, yVal) ->
//                val finalX = xVal * scaleFactor
//                val finalY = yVal * scaleFactor
//                val topLeft = Offset(finalX - halfW, finalY - halfH)
//                drawImage(
//                    image = towerBitmap,
//                    srcSize = IntSize(towerBitmap.width, towerBitmap.height),
//                    dstSize = IntSize(towerW, towerH),
//                    dstOffset = IntOffset(topLeft.x.toInt(), topLeft.y.toInt())
//                )
//            }
            // Draw the live position as a red dot.
            val (lx, ly) = liveCoord
            drawCircle(
                color = Color.Red,
                radius = 10f,
                center = Offset(lx * scaleFactor, ly * scaleFactor)
            )
        }
    }
}

/**
 * Returns the device's IPv4 address.
 */
fun findLocalIP(context: android.content.Context): String {
    return try {
        val cm = context.getSystemService(android.content.Context.CONNECTIVITY_SERVICE) as? ConnectivityManager
            ?: return "0.0.0.0"
        val network: Network = cm.activeNetwork ?: return "0.0.0.0"
        val lp: LinkProperties = cm.getLinkProperties(network) ?: return "0.0.0.0"
        for (la in lp.linkAddresses) {
            val addr = la.address
            if (addr is Inet4Address && !addr.isLoopbackAddress) {
                return addr.hostAddress ?: "0.0.0.0"
            }
        }
        "0.0.0.0"
    } catch (e: Exception) {
        e.printStackTrace()
        "0.0.0.0"
    }
}

/**
 * A never-ending TCP server that listens on [port] for live (x,y) updates and ALSA mic connection messages.
 *
 * Expected messages:
 *   - For live coordinate updates: "x,y"
 *   - For ALSA mic connection: "alsaConnected,<micIndex>"
 */
fun startTcpServerForever(
    port: Int,
    onCoord: (Float, Float) -> Unit,
    onMicConnected: (Int) -> Unit,
    OnClassifcation: (String) -> Unit
    onQuadrant: (Int) -> Unit
) {
    println("Starting TCP server on port $port for live updates and mic connection messages...")
    var server: ServerSocket? = null
    try {
        server = ServerSocket(port)
        while (true) {
            try {
                println("Waiting for client connection on port $port...")
                val client = server.accept()
                println("Client connected: ${client.inetAddress?.hostAddress}")
                val reader = BufferedReader(InputStreamReader(client.getInputStream()))
                while (true) {
                    val line = reader.readLine() ?: break
                    // Check if this is an ALSA connection message.
                    if (line.startsWith("alsaConnected")) {
                        val parts = line.split(",")
                        if (parts.size >= 2) {
                            val micIndex = parts[1].toIntOrNull() ?: continue
                            onMicConnected(micIndex)
                        }
                        continue
                    }
                    // Check for classification message.
                    if (line.startsWith("classification")) {
                        val parts = line.split(",", limit = 2)
                        if (parts.size >= 2) {
                            val classification = parts[1]
                            OnClassifcation(classification)
                        }
                        continue
                    }
                    if (line.startsWith("location")) {
                        // Otherwise, treat as live coordinate update.
                        val parts = line.split(",")
                        if (parts.size >= 2) {  // Shouldn't this be >=3 ??
                            val x = parts[1].toFloatOrNull() ?: 0f
                            val y = parts[2].toFloatOrNull() ?: 0f
                            onCoord(x, y)
                        }
                    }
                    if (line.startsWith("quadrant")) {
                        // Treat as quadrant information
                        val parts = line.split(",")
                        if (parts.size == 2) {
                            val quadrant = parts[1].toInt()
                        }

                    }
                }
                println("Client disconnected.")
                client.close()
            } catch (ex: Exception) {
                ex.printStackTrace()
            }
        }
    } catch (ex: Exception) {
        ex.printStackTrace()
    } finally {
        println("TCP server shutting down.")
        server?.close()
    }
}

/**
 * Helper function to send a location update for a given tower.
 *
 * Sends a message of the form "sendLocation,<micIndex>,<x>,<y>\n" to the Pi at [destIp] on port 39440.
 */
fun sendLocationForTower(micIndex: Int, x: String, y: String, destIp: String) {
    try {
        val socket = Socket(destIp, 39440)
        val msg = "sendLocation,$micIndex,$x,$y\n"
        socket.getOutputStream().write(msg.toByteArray())
        socket.close()
        println("Sent location for mic $micIndex: ($x, $y)")
    } catch (e: Exception) {
        e.printStackTrace()
    }
}

@Preview(showBackground = true, widthDp = 800, heightDp = 400)
@Composable
fun PreviewMySplitScreen() {
    SPARTN1Theme {
        MySplitScreen()
    }
}