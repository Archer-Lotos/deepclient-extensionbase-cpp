from .deep_client import DeepClient
from .deep_client_options import DeepClientOptions

__all__ = ['DeepClient', 'DeepClientOptions']

def make_deep_client(token, url):
    if not token:
        raise ValueError("No token provided")
    if not url:
        raise ValueError("No url provided")
    return DeepClient()
