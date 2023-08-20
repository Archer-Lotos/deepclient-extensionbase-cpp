from typing import Any, Optional, Union, Dict, List
from deepclient import DeepClient, DeepClientOptions
from gql import gql, Client
from gql.transport.aiohttp import AIOHTTPTransport

global_deep_client = None
def make_deep_client(token, url):
    global global_deep_client

    if global_deep_client is None:
        if not token:
            raise ValueError("No token provided")
        if not url:
            raise ValueError("No url provided")

        transport = AIOHTTPTransport(url=url, headers={'Authorization': f"Bearer {token}"})
        client = Client(transport=transport, fetch_schema_from_transport=True)
        options = DeepClientOptions(gql_client=client)
        global_deep_client = DeepClient(options)

    return global_deep_client

async def select(token, url, exp: Union[Dict, int, List[int]], options: Dict = {}) -> Dict:
    return await make_deep_client(token, url).select(exp, options)

async def insert(token, url, objects, options: Dict = {}) -> Dict:
    return await make_deep_client(token, url).insert(objects, options)

async def update(token, url, exp: Dict, value: Dict, options: Dict = {}) -> Dict:
    return await make_deep_client(token, url).update(exp, value, options)

async def delete(token, url, exp: Union[Dict, int, List[int]], options: Dict = {}) -> Dict:
    return await make_deep_client(token, url).delete(exp, options)

async def serial(token, url, AsyncSerialParams: Dict):
    return await make_deep_client(token, url).serial(AsyncSerialParams)

async def reserve(token, url):
    return await make_deep_client(token, url).reserve()

async def wait_for(token, url):
    return await make_deep_client(token, url).wait_for()

async def id(token, url, start: Any, *path: Any) -> int:
    return await make_deep_client(token, url).id(start, path)

def id_local(token, url):
    return make_deep_client(token, url).id_local()

async def guest(token, url):
    return await make_deep_client(token, url).guest()

async def jwt(token, url):
    return await make_deep_client(token, url).jwt()

async def whoami(token, url):
    return await make_deep_client(token, url).whoami()

async def login(token, url):
    return await make_deep_client(token, url).login()

async def logout(token, url):
    return await make_deep_client(token, url).logout()

async def can(token, url):
    return await make_deep_client(token, url).can()

async def name(token, url):
    return await make_deep_client(token, url).name()

def name_local(token, url):
    return make_deep_client(token, url).name_local()
