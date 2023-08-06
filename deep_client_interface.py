from typing import Any, Optional, Union, Dict, List


class DeepClient:
    _ids = {}
    _serialize = {}

    async def select(self, exp: Union[Dict, int, List[int]], options: Dict = {}) -> Dict:
        return {"error": {"message": "!exp"}, "data": None, "loading": False, "networkStatus": None}

    async def insert(self, objects, options: Dict = {}) -> Dict:
        return {"error": {"message": "!record"}, "data": None, "loading": False, "networkStatus": None}

    async def update(self, exp: Dict, value: Dict, options: Dict = {}) -> Dict:
        return {"error": {"message": "!exp or !value"}, "data": None, "loading": False,
                "networkStatus": None}

    async def delete(self, exp: Union[Dict, int, List[int]], options: Dict = {}) -> Dict:
        return {}

    async def serial(self, AsyncSerialParams: Dict):
        return {"error": "test"}

    async def reserve(self):
        raise NotImplementedError("Method not implemented")

    async def wait_for(self):
        raise NotImplementedError("Method not implemented")

    async def id(self, start: Any, *path: Any) -> int:
        return 1

    def id_local(self):
        raise NotImplementedError("Method not implemented")

    async def guest(self):
        raise NotImplementedError("Method not implemented")

    async def jwt(self):
        raise NotImplementedError("Method not implemented")

    async def whoami(self):
        raise NotImplementedError("Method not implemented")

    async def login(self):
        raise NotImplementedError("Method not implemented")

    async def logout(self):
        raise NotImplementedError("Method not implemented")

    async def can(self):
        raise NotImplementedError("Method not implemented")

    async def name(self):
        raise NotImplementedError("Method not implemented")

    def name_local(self):
        raise NotImplementedError("Method not implemented")


def make_deep_client(token, url):
    if not token:
        raise ValueError("No token provided")
    if not url:
        raise ValueError("No url provided")
    return DeepClient()