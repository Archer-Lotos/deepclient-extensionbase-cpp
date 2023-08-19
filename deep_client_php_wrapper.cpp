#include "phpcpp.h"
#include <string>
#include <Python.h>

class DeepClientPhpWrapper : public Php::Base {
public:
    DeepClientPhpWrapper() {
        Py_Initialize();
        PyObject* deepclientModule = PyImport_ImportModule("deepclient");
        PyObject* gqlModule = PyImport_ImportModule("gql");
        PyObject* aiohttpModule = PyImport_ImportModule("gql.transport.aiohttp");

        PyObject* deepClientClass = PyObject_GetAttrString(deepclientModule, "DeepClient");
        PyObject* deepClientOptionsClass = PyObject_GetAttrString(deepclientModule, "DeepClientOptions");
        PyObject* gqlClass = PyObject_GetAttrString(gqlModule, "gql");
        PyObject* clientClass = PyObject_GetAttrString(gqlModule, "Client");
        PyObject* aiohttpTransportClass = PyObject_GetAttrString(aiohttpModule, "AIOHTTPTransport");
    }

    ~DeepClientPhpWrapper() {
        Py_XDECREF(deepClientClass);
        Py_XDECREF(deepClientOptionsClass);
        Py_XDECREF(gqlClass);
        Py_XDECREF(clientClass);
        Py_XDECREF(aiohttpTransportClass);

        Py_DECREF(deepclientModule);
        Py_DECREF(gqlModule);
        Py_DECREF(aiohttpModule);
        Py_Finalize();
    }

    void __construct(Php::Parameters &params) {
        if (!params[0].isNull() && !params[1].isNull()) {
            Php::Value token = params[0];
            Php::Value url = params[1];
        } else {
            throw Php::Exception("Both token and url are required.");
        }
    }

    Php::Value select(Php::Parameters &params) {
        return call_python_function("select", params[0]);
    }

    Php::Value insert(Php::Parameters &params) {
        return call_python_function("insert", params[0]);
    }

    Php::Value call_python_function(const std::string& function_name, const Php::Value& query) {
        Php::Value result;
        if (deepclientModule && deepClientClass) {
            PyObject* pyFunc = PyObject_GetAttrString(deepClientClass, function_name.c_str());
            if (pyFunc && PyCallable_Check(pyFunc)) {
                PyObject* pyArgs = PyTuple_Pack(1, PyUnicode_DecodeFSDefault(query.stringValue().c_str()));
                PyObject* pyResult = PyObject_CallObject(pyFunc, pyArgs);
                if (pyResult) {
                    result = Php::Value(PyUnicode_AsUTF8(pyResult));
                    Py_DECREF(pyResult);
                } else {
                    PyErr_Print();
                }
                Py_DECREF(pyArgs);
            } else {
                PyErr_Print();
            }
            Py_XDECREF(pyFunc);
        }
        return result;
    }

private:
    PyObject* deepclientModule = nullptr;
    PyObject* gqlModule = nullptr;
    PyObject* aiohttpModule = nullptr;

    PyObject* deepClientClass = nullptr;
    PyObject* deepClientOptionsClass = nullptr;
    PyObject* gqlClass = nullptr;
    PyObject* clientClass = nullptr;
    PyObject* aiohttpTransportClass = nullptr;
};


extern "C" {
    PHPCPP_EXPORT void *get_module() {
        static Php::Extension extension("deep_client_php_extension", "1.0");

        Php::Class<DeepClientPhpWrapper> deepClientPhpWrapper("DeepClientPhpWrapper");

        deepClientPhpWrapper.method("__construct", &DeepClientPhpWrapper::__construct, {
            Php::ByVal("token", Php::Type::String),
            Php::ByVal("url", Php::Type::String)
        });

        deepClientPhpWrapper.method<&DeepClientPhpWrapper::select>("select", {
            Php::ByVal("query", Php::Type::String)
        });
        deepClientPhpWrapper.method<&DeepClientPhpWrapper::insert>("insert", {
            Php::ByVal("query", Php::Type::String)
        });
        extension.add(std::move(deepClientPhpWrapper));

        return extension;
    }
}