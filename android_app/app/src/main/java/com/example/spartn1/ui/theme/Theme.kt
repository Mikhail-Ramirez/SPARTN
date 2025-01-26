package com.example.spartn1.ui.theme

import androidx.compose.foundation.isSystemInDarkTheme
import androidx.compose.material.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.sp

// ------------ Colors for M2 ------------
private val DarkColorPalette = darkColors(
    primary = Color(0xFFBB86FC),
    primaryVariant = Color(0xFF3700B3),
    secondary = Color(0xFF03DAC6)
)

private val LightColorPalette = lightColors(
    primary = Color(0xFF6200EE),
    primaryVariant = Color(0xFF3700B3),
    secondary = Color(0xFF03DAC6)
)

// ------------ Typography for M2 ------------
val AppTypography = Typography(
    body1 = TextStyle(
        fontFamily = FontFamily.Default,
        fontWeight = FontWeight.Normal,
        fontSize = 16.sp
    )
    // Add other styles if you wish, e.g. h1, h2, etc.
)

// ------------ Shapes for M2 ------------
val AppShapes = Shapes()

// ------------ Theme Composable ------------
@Composable
fun SPARTN1Theme(
    darkTheme: Boolean = isSystemInDarkTheme(),
    content: @Composable () -> Unit
) {
    // Pick dark or light palette
    val colors = if (darkTheme) DarkColorPalette else LightColorPalette

    // Apply Material 2 theme
    MaterialTheme(
        colors = colors,
        typography = AppTypography,
        shapes = AppShapes,
        content = content
    )
}
