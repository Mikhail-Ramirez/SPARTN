package com.example.spartn1

import android.content.pm.ActivityInfo
import android.graphics.BitmapFactory
import android.net.ConnectivityManager
import android.net.LinkProperties
import android.net.Network
import android.net.wifi.WifiManager
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
import kotlinx.coroutines.withContext
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.*
import kotlin.math.roundToInt
// Add necessary imports at the top:
import java.net.Socket

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // Force landscape
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

    // Four coordinate pairs
    val x1 = remember { mutableStateOf(TextFieldValue("0")) }
    val y1 = remember { mutableStateOf(TextFieldValue("0")) }
    val x2 = remember { mutableStateOf(TextFieldValue("0")) }
    val y2 = remember { mutableStateOf(TextFieldValue("0")) }
    val x3 = remember { mutableStateOf(TextFieldValue("0")) }
    val y3 = remember { mutableStateOf(TextFieldValue("0")) }
    val x4 = remember { mutableStateOf(TextFieldValue("0")) }
    val y4 = remember { mutableStateOf(TextFieldValue("0")) }

    val userCoords = listOf(
        x1.value.text to y1.value.text,
        x2.value.text to y2.value.text,
        x3.value.text to y3.value.text,
        x4.value.text to y4.value.text
    ).map { (xs, ys) ->
        (xs.toFloatOrNull() ?: 0f) to (ys.toFloatOrNull() ?: 0f)
    }

    // Live Pi coordinate
    val liveCoord = remember { mutableStateOf(0f to 0f) }

    // Get IP from connectivity manager
    val context = LocalContext.current
    val localIp = remember { findLocalIP(context) } // "0.0.0.0" if none
    val serverPort = 39439

    // Start server
    val scope = rememberCoroutineScope()
    LaunchedEffect(Unit) {
        scope.launch(Dispatchers.IO) {
            startTcpServerForever(serverPort) { x, y ->
                liveCoord.value = x to y
            }
        }
    }

    if (!showBlueScreen) {
        // GREEN SCREEN
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
                    // Left Pane
                    Column(
                        modifier = Modifier
                            .weight(1f)
                            .fillMaxHeight()
                            .padding(16.dp),
                        verticalArrangement = Arrangement.Top,
                        horizontalAlignment = Alignment.CenterHorizontally
                    ) {
                        Button(
                            onClick = { showBlueScreen = true },
                            modifier = Modifier.padding(bottom = 16.dp)
                        ) {
                            Text("Switch to Blue Screen")
                        }

                        Text("Type (x, y) coords of towers", style = MaterialTheme.typography.h6)
                        Spacer(modifier = Modifier.height(8.dp))
                        CoordinateRow("Coord 1", x1, y1)
                        CoordinateRow("Coord 2", x2, y2)
                        CoordinateRow("Coord 3", x3, y3)
                        CoordinateRow("Coord 4", x4, y4)
                    }

                    // Right Pane: tower plane
                    Box(
                        modifier = Modifier
                            .weight(1f)
                            .fillMaxHeight()
                    ) {
                        TowerPlane(userCoords, liveCoord.value)
                    }
                }

                // Bottom bar: IP + Port
                Text(
                    text = "Listening on $localIp:$serverPort",
                    modifier = Modifier
                        .fillMaxWidth()
                        .background(Color.LightGray)
                        .padding(8.dp)
                )
            }
        }
    } else {
        // BLUE SCREEN: New UI with IP input and buttons for sending coordinates
        BlueScreen(onSwitchScreen = { showBlueScreen = false })
    }
}

/**
 * Row for (X, Y) text fields
 */
@Composable
fun CoordinateRow(
    label: String,
    xState: MutableState<TextFieldValue>,
    yState: MutableState<TextFieldValue>
) {
    Column(horizontalAlignment = Alignment.CenterHorizontally) {
        Text(text = label, style = MaterialTheme.typography.subtitle1)
        Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
            TextField(
                value = xState.value,
                onValueChange = { xState.value = it },
                label = { Text("X") },
                keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number),
                modifier = Modifier.width(80.dp)
            )
            TextField(
                value = yState.value,
                onValueChange = { yState.value = it },
                label = { Text("Y") },
                keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number),
                modifier = Modifier.width(80.dp)
            )
        }
    }
}

/**
 * New composable for the blue screen.
 * It includes:
 * - A button to switch back to the green screen.
 * - An input field to specify the destination IP.
 * - Three buttons to send coordinate pairs "1,1", "2,2", and "3,3".
 */
@Composable
fun BlueScreen(onSwitchScreen: () -> Unit) {
    val coroutineScope = rememberCoroutineScope()
    var destIp by remember { mutableStateOf(TextFieldValue("100.76.15.100")) } // default value; change as needed

    Column(
        modifier = Modifier
            .fillMaxSize()
            .background(Color.Blue)
            .padding(16.dp),
        verticalArrangement = Arrangement.Top,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        // Button to switch back to the green screen
        Button(
            onClick = { onSwitchScreen() },
            modifier = Modifier.fillMaxWidth()
        ) {
            Text("Switch to Green Screen")
        }
        Spacer(modifier = Modifier.height(16.dp))
        
        // Input field for the destination IP address
        TextField(
            value = destIp,
            onValueChange = { destIp = it },
            label = { Text("Destination IP") },
            modifier = Modifier.fillMaxWidth()
        )
        Spacer(modifier = Modifier.height(16.dp))
        
        // Button to send "1,1"
        Button(
            onClick = {
                coroutineScope.launch(Dispatchers.IO) {
                    try {
                        Socket(destIp.text, 39439).use { socket ->
                            socket.getOutputStream().write("1,1\n".toByteArray())
                        }
                    } catch (e: Exception) {
                        e.printStackTrace()
                    }
                }
            },
            modifier = Modifier.fillMaxWidth()
        ) {
            Text("Send 1,1")
        }
        Spacer(modifier = Modifier.height(8.dp))
        
        // Button to send "2,2"
        Button(
            onClick = {
                coroutineScope.launch(Dispatchers.IO) {
                    try {
                        Socket(destIp.text, 39439).use { socket ->
                            socket.getOutputStream().write("2,2\n".toByteArray())
                        }
                    } catch (e: Exception) {
                        e.printStackTrace()
                    }
                }
            },
            modifier = Modifier.fillMaxWidth()
        ) {
            Text("Send 2,2")
        }
        Spacer(modifier = Modifier.height(8.dp))
        
        // Button to send "3,3"
        Button(
            onClick = {
                coroutineScope.launch(Dispatchers.IO) {
                    try {
                        Socket(destIp.text, 39439).use { socket ->
                            socket.getOutputStream().write("3,3\n".toByteArray())
                        }
                    } catch (e: Exception) {
                        e.printStackTrace()
                    }
                }
            },
            modifier = Modifier.fillMaxWidth()
        ) {
            Text("Send 3,3")
        }
    }
}


/**
 * Draw green plane, show user coords as towers, plus a red dot for liveCoord.
 */
@Composable
fun TowerPlane(coords: List<Pair<Float, Float>>, liveCoord: Pair<Float, Float>) {
    val scaleFactor = 10f
    val context = LocalContext.current

    // Load tower.png
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
            // Axes
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

            // Towers
            coords.forEach { (xVal, yVal) ->
                val finalX = xVal * scaleFactor
                val finalY = yVal * scaleFactor
                val topLeft = Offset(finalX - halfW, finalY - halfH)
                drawImage(
                    image = towerBitmap,
                    srcSize = IntSize(towerBitmap.width, towerBitmap.height),
                    dstSize = IntSize(towerW, towerH),
                    dstOffset = IntOffset(topLeft.x.toInt(), topLeft.y.toInt())
                )
            }

            // RPi dot
            val (lx, ly) = liveCoord
            val dotX = lx * scaleFactor
            val dotY = ly * scaleFactor
            drawCircle(
                color = Color.Red,
                radius = 10f,
                center = Offset(dotX, dotY)
            )
        }
    }
}

/**
 * Returns the device's IPv4 address using ConnectivityManager + LinkProperties.
 * If none found, returns "0.0.0.0".
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
 * A never-ending TCP server on [port]. Accept client, parse "x,y" lines,
 * pass them to [onCoord]. If the client closes, wait for next client.
 */
fun startTcpServerForever(port: Int, onCoord: (Float, Float) -> Unit) {
    println("startTcpServerForever on port $port")
    var server: ServerSocket? = null
    try {
        server = ServerSocket(port)
        while (true) {
            try {
                println("Waiting for client on port $port...")
                val client = server.accept()
                println("Client connected: ${client.inetAddress?.hostAddress}")

                val reader = BufferedReader(InputStreamReader(client.getInputStream()))
                while (true) {
                    val line = reader.readLine() ?: break
                    val parts = line.split(",")
                    if (parts.size == 2) {
                        val x = parts[0].toFloatOrNull() ?: 0f
                        val y = parts[1].toFloatOrNull() ?: 0f
                        onCoord(x, y)
                    }
                }
                println("Client disconnected.")
                client.close()
            } catch (ex: Exception) {
                ex.printStackTrace()
                // keep going for next client
            }
        }
    } catch (ex: Exception) {
        ex.printStackTrace()
    } finally {
        println("Exiting server? - Shouldn’t happen unless app killed.")
        server?.close()
    }
}

@Preview(showBackground = true, widthDp = 800, heightDp = 400)
@Composable
fun PreviewMySplitScreen() {
    SPARTN1Theme {
        MySplitScreen()
    }
}
