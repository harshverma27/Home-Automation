package com.example.relaycontrol

import android.os.Bundle
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.relaycontrol.ui.theme.RelaycontrolTheme
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
                    RelayControlApp(modifier = Modifier.padding(innerPadding))
                }
            }
        }
    }
}

@Composable
fun RelayControlApp(modifier: Modifier = Modifier.background(color = Color(167, 144, 165))) {
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

    Card(
        modifier = modifier
            .padding(8.dp)
            .fillMaxHeight(),
        colors = CardDefaults.cardColors(containerColor = Color(197, 235, 195))
    ) {
        Column(
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center,
            modifier = Modifier
                .fillMaxHeight()
                .padding(16.dp)
        ) {
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

@Preview(showBackground = true)
@Composable
fun RelayControlPreview() {
    RelaycontrolTheme {
        RelayControlApp()
    }
}
