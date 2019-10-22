import asyncio
import websockets

TEST_URI = "ws://127.0.0.1:2222"
TEST_MSG = "Hello, Router!"

async def test():
    async with websockets.connect(TEST_URI) as websocket:
        print(f"> {TEST_MSG}")
        await websocket.send(TEST_MSG)
        response = await websocket.recv()
        print(f"< {response}")

asyncio.get_event_loop().run_until_complete(test())
