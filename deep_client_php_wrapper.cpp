#include "phpcpp.h"
#include <string>
#include <Python.h>

class DeepClientPhpWrapper : public Php::Base {
public:
    DeepClientPhpWrapper() {}

    ~DeepClientPhpWrapper() {
        Py_XDECREF(deepClientClass);
        Py_Finalize();
    }

    void __construct(Php::Parameters &params) {
        if (!params[0].isNull() && !params[1].isNull()) {
            const std::string& token = params[0];
            const std::string& url = params[1];

            Py_Initialize();

            PyObject* deepClientModule = PyImport_ImportModule("deepclient");
            PyObject* gqlModule = PyImport_ImportModule("gql");
            PyObject* aiohttpModule = PyImport_ImportModule("gql.transport.aiohttp");

            PyObject* deepClientClass = PyObject_GetAttrString(deepClientModule, "DeepClient");
            PyObject* deepClientOptionsClass = PyObject_GetAttrString(deepClientModule, "DeepClientOptions");
            PyObject* gqlClass = PyObject_GetAttrString(gqlModule, "gql");
            PyObject* clientClass = PyObject_GetAttrString(gqlModule, "Client");
            PyObject* aiohttpTransportClass = PyObject_GetAttrString(aiohttpModule, "AIOHTTPTransport");

            if (deepClientModule && gqlModule && aiohttpModule) {
                throw Php::Exception("Profit.");
            } else {
                throw Php::Exception("Failed to import required Python modules");
            }
            /*transport = Php::Object("AIOHTTPTransport", url, "Authorization: Bearer " + token);
            client = Php::Object("Client", transport, true);
            options = Php::Object("DeepClientOptions", client);
            deepClient_ = Php::Object("DeepClient", options);*/

            Py_XDECREF(deepClientOptionsClass);
            Py_XDECREF(gqlClass);
            Py_XDECREF(clientClass);
            Py_XDECREF(aiohttpTransportClass);

            Py_XDECREF(deepClientModule);
            Py_XDECREF(gqlModule);
            Py_XDECREF(aiohttpModule);
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
        if (deepClientModule && deepClientClass) {
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
    PyObject* deepClientModule = nullptr;
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

        deepClientPhpWrapper.method<&DeepClientPhpWrapper::__construct>("__construct", {
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