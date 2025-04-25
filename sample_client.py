import asyncio
import websockets
import json

async def client():
    uri = "wss://alexcoder23.ru/wnet/ws"
    
    headers = {
        'Cookie': 'auth="type=password&auth=$auth_data$"'
    }
    
    async with websockets.connect(uri, additional_headers=headers) as websocket:
        bot_id = input("Введите id бота: ")
        await websocket.send(f"connect@{bot_id}")
        response = await websocket.recv()
        print(f"Connection response: {response}")
        
        await websocket.send(f"cssend@{bot_id}@{input('Введите сообщение: ')}")
        
        async for message in websocket:
            print(f"Client received: {message}")
            if message.startswith("mess@"):
                parts = message.split("@")
                bot_id = parts[1]
                bot_msg = "@".join(parts[2:])
                print(f"Message from bot {bot_id}: {bot_msg}")

asyncio.get_event_loop().run_until_complete(client())
