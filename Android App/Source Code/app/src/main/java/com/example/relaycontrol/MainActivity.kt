package com.example.relaycontrol

import android.os.Bundle
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.painter.Painter
import androidx.compose.ui.node.ModifierNodeElement
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.relaycontrol.ui.theme.RelaycontrolTheme
import kotlinx.coroutines.delay
import okhttp3.OkHttpClient
import okhttp3.Request
import kotlin.concurrent.thread

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            RelaycontrolTheme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    RelayControlApp(modifier = Modifier.padding(innerPadding).background(color = Color(34, 124, 157)))
                }
            }
        }
    }
}

@Composable
fun RelayControlApp(modifier: Modifier = Modifier) {
    val context = LocalContext.current
    val client = OkHttpClient()


    // State to track if each relay is ON or OFF
    var relay1State by remember { mutableStateOf(false) }
    var relay2State by remember { mutableStateOf(false) }
    var relay3State by remember { mutableStateOf(false) }
    var relay4State by remember { mutableStateOf(false) }

    Column(
        modifier = modifier
            .fillMaxSize()
            .padding(16.dp),
        verticalArrangement = Arrangement.SpaceBetween
    ) {

        HeaderSection(modifier = Modifier)
        Spacer(modifier = Modifier.height(10.dp))
        // Upper row: Relay 1 (left) and Relay 2 (right)
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .weight(1f)
        ) {
            RelayButtonAndIndicator(
                relayNumber = 1,
                isRelayOn = relay1State,
                onRelayChange = { relay1State = it },
                client = client,
                context = context,
                modifier = Modifier.weight(1f)
            )

            Spacer(modifier = Modifier.width(16.dp))

            RelayButtonAndIndicator(
                relayNumber = 2,
                isRelayOn = relay2State,
                onRelayChange = { relay2State = it },
                client = client,
                context = context,
                modifier = Modifier.weight(1f)
            )
        }

        Spacer(modifier = Modifier.height(16.dp))

        // Lower row: Relay 3 (left) and Relay 4 (right)
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .weight(1f)
        ) {
            RelayButtonAndIndicator(
                relayNumber = 3,
                isRelayOn = relay3State,
                onRelayChange = { relay3State = it },
                client = client,
                context = context,
                modifier = Modifier.weight(1f)
            )

            Spacer(modifier = Modifier.width(16.dp))

            RelayButtonAndIndicator(
                relayNumber = 4,
                isRelayOn = relay4State,
                onRelayChange = { relay4State = it },
                client = client,
                context = context,
                modifier = Modifier.weight(1f)
            )
        }
    }
}

@Composable
fun RelayButtonAndIndicator(
    relayNumber: Int,
    isRelayOn: Boolean,
    onRelayChange: (Boolean) -> Unit,
    client: OkHttpClient,
    context: android.content.Context,
    modifier: Modifier = Modifier
) {
    var localRelayState by remember { mutableStateOf(isRelayOn) }
    var elapsedTime by remember { mutableStateOf(0) } // Timer in seconds

    // Launch the timer when relay is ON
    LaunchedEffect(localRelayState) {
        while (localRelayState) {
            delay(1000L) // Wait for 1 second
            elapsedTime++ // Increase time by 1 second
        }
    }

    Card(
        modifier = modifier
            .padding(8.dp)
            .fillMaxHeight(),
        colors = CardDefaults.cardColors(containerColor = Color(23, 195, 178))
    ) {
        Column(
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center,
            modifier = Modifier
                .fillMaxHeight()
                .padding(16.dp)
        ) {
            // Timer Display
            Card(
                modifier = Modifier
                    .fillMaxWidth()
                    .height(50.dp)
                    .align(Alignment.CenterHorizontally),
                colors = CardDefaults.cardColors(containerColor = Color(255, 203, 119)) // Optional background color
            ) {
                Column(
                    modifier = Modifier
                        .fillMaxSize(),
                    horizontalAlignment = Alignment.CenterHorizontally, // Centers children horizontally
                    verticalArrangement = Arrangement.Center // Centers children vertically
                ) {
                    Text(
                        text = "Time Since ON:",
                        color = Color.Black,
                        fontSize = 14.sp
                    )
                    Text(
                        text = "${elapsedTime / 60}m ${elapsedTime % 60}s",
                        color = Color.Black,
                        fontSize = 16.sp
                    )
                }
            }


            Spacer(modifier = Modifier.height(8.dp))

            // Relay status indicator (circle)
            Box(
                modifier = Modifier
                    .size(50.dp)
                    .background(
                        color = if (localRelayState) Color.Green else Color.Red,
                        shape = CircleShape
                    )
            )

            Spacer(modifier = Modifier.height(8.dp))

            // ON Button
            Button(onClick = {
                sendRequest("http://192.168.137.138/relay$relayNumber/on", client, context) { success ->
                    if (success) {
                        localRelayState = true
                        elapsedTime = 0 // Reset the timer when relay turns ON
                        onRelayChange(true)
                    }
                }
            }) {
                Text(text = "Relay $relayNumber ON", textAlign = TextAlign.Center)
            }

            Spacer(modifier = Modifier.height(8.dp))

            // OFF Button
            Button(onClick = {
                sendRequest("http://192.168.137.138/relay$relayNumber/off", client, context) { success ->
                    if (success) {
                        localRelayState = false
                        onRelayChange(false)
                    }
                }
            }) {
                Text(text = "Relay $relayNumber OFF", textAlign = TextAlign.Center)
            }
        }
    }
}


fun sendRequest(url: String, client: OkHttpClient, context: android.content.Context, onResult: (Boolean) -> Unit) {
    thread {
        try {
            val request = Request.Builder().url(url).build()
            val response = client.newCall(request).execute()
            val success = response.isSuccessful

            (context as ComponentActivity).runOnUiThread {
                if (success) {
                    Toast.makeText(context, "Request Sent: ${url.split("/").last()}", Toast.LENGTH_SHORT).show()
                } else {
                    Toast.makeText(context, "Failed to send request!", Toast.LENGTH_SHORT).show()
                }
                onResult(success)
            }
        } catch (e: Exception) {
            (context as ComponentActivity).runOnUiThread {
                Toast.makeText(context, "Error: ${e.message}", Toast.LENGTH_SHORT).show()
                onResult(false)
            }
        }
    }
}
@Composable
fun HeaderSection(modifier: Modifier = Modifier) {
    Card(
        modifier = modifier.fillMaxWidth(),
        colors = CardDefaults.cardColors(containerColor = Color(207, 92, 54))
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(0.dp),
            horizontalArrangement = Arrangement.SpaceEvenly, // Centers content horizontally
            verticalAlignment = Alignment.CenterVertically // Centers content vertically
        ) {
            Image(
                painter = painterResource(id = R.drawable.img), // Replace with your image name
                contentDescription = "Logo",
                modifier = Modifier.size(75.dp) // Adjust size as needed
            )
            Text(
                text = "Designed By SPEC",
                textAlign = TextAlign.Center, fontSize = 16.5.sp
            )

        }
    }
}

@Preview(showBackground = true)
@Composable
fun RelayControlPreview() {
    RelaycontrolTheme {
        RelayControlApp()
    }
}
