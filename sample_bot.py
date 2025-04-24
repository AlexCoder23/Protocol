import asyncio
import websockets
import json

async def bot():
    uri = "wss://alexcoder23.ru/wnet/ws"
    
    headers = {
        'Cookie': 'auth="type=password&auth=$auth_data$"'
    }
    
    async with websockets.connect(uri, additional_headers=headers) as websocket:
        await websocket.send("add@testbot")
        response = await websocket.recv()
        print(f"Received after add: {response}")
        async for message in websocket:
            print(f"Bot received: {message}")
            if message.startswith("mess@"):
                parts = message.split("@")
                client_sid = parts[1]
                client_msg = "@".join(parts[2:])
                print(f"Message from client {client_sid}: {client_msg}")
                reply = f"Hello from bot! You said: {client_msg}"
                await websocket.send(f"scsend@{client_sid}@{reply}")
                
asyncio.get_event_loop().run_until_complete(bot())
