#include "phpcpp.h"
#include <string>
#include <Python.h>

class DeepClientPhpWrapper : public Php::Base {
public:
    DeepClientPhpWrapper() {
        Py_Initialize();
    }

    ~DeepClientPhpWrapper() {
        Py_XDECREF(deepClientOptionsClass);
        Py_XDECREF(gqlClass);
        Py_XDECREF(clientClass);
        Py_XDECREF(aiohttpTransportClass);

        Py_XDECREF(deepClientModule);
        Py_XDECREF(gqlModule);
        Py_XDECREF(aiohttpModule);
        Py_XDECREF(deepClientClass);
        Py_XDECREF(deepClient);
        Py_Finalize();
    }

    void __construct(Php::Parameters &params) {
        if (!params[0].isNull() && !params[1].isNull()) {
            const std::string& token = params[0];
            const std::string& url = params[1];

            Py_Initialize();

            /*PyObject* deepClientModule = PyImport_ImportModule("deepclient");
            PyObject* gqlModule = PyImport_ImportModule("gql");
            PyObject* aiohttpModule = PyImport_ImportModule("gql.transport.aiohttp");

            PyObject* deepClientClass = PyObject_GetAttrString(deepClientModule, "DeepClient");
            PyObject* deepClientOptionsClass = PyObject_GetAttrString(deepClientModule, "DeepClientOptions");
            PyObject* gqlClass = PyObject_GetAttrString(gqlModule, "gql");
            PyObject* clientClass = PyObject_GetAttrString(gqlModule, "Client");
            PyObject* aiohttpTransportClass = PyObject_GetAttrString(aiohttpModule, "AIOHTTPTransport");

            if (deepClientModule && gqlModule && aiohttpModule) {
                PyObject* aiohttpTransportArgs = PyTuple_Pack(2,
                    Py_BuildValue("s", url.c_str()),
                    Py_BuildValue("s", ("Authorization: Bearer " + token).c_str())
                );
                PyObject* aiohttpTransportInstance = PyObject_CallObject(aiohttpTransportClass, aiohttpTransportArgs);
                PyObject* clientArgs = PyTuple_Pack(2, aiohttpTransportInstance, Py_True);
                PyObject* clientInstance = PyObject_CallObject(clientClass, clientArgs);

                //PyObject* deepClientOptionsArgs = PyTuple_Pack(1, clientInstance);
                PyObject* deepClientOptionsInstance = PyObject_CallObject(deepClientOptionsClass, clientInstance);

                //PyObject* deepClientArgs = PyTuple_Pack(1, deepClientOptionsInstance);
                PyObject* deepClient = PyObject_CallObject(deepClientClass, deepClientOptionsInstance);
                // throw Php::Exception("Profit.");
            } else {
                throw Php::Exception("Failed to import required Python modules");
            }*/
        } else {
            throw Php::Exception("Both token and url are required.");
        }
    }

    Php::Value getPyObject(Php::Parameters &params) {
        if (!params[0].isNull() && !params[1].isNull()) {
            const std::string& token = params[0];
            const std::string& url = params[1];

            Py_Initialize();
            PyRun_SimpleString("import sys\n"
                                "sys.path.append('.');");
            PyObject* makeDeepClientModule = PyImport_ImportModule("make_deep");
            PyObject* makeDeepClientFunc = PyObject_GetAttrString(makeDeepClientModule, "make_deep_client");

            if (makeDeepClientFunc, PyCallable_Check(makeDeepClientFunc)) {
                PyObject* aiohttpTransportArgs = PyTuple_Pack(2,
                    Py_BuildValue("s", token.c_str()),
                    Py_BuildValue("s", url.c_str())
                );

                PyObject* deepClient = PyObject_CallObject(makeDeepClientFunc, aiohttpTransportArgs);

                if (deepClient) {
                    return 9;
                } else {
                    throw Php::Exception("deepClient");
                }

                /*const std::string& cppSelect = "select";
                PyObject* deepClientSelectFunc = PyObject_CallObject(deepClient, PyUnicode_DecodeFSDefault(cppSelect.c_str()));
                const std::string& selectArgs = "1";
                PyObject* pyResult = PyObject_CallObject(deepClientSelectFunc, PyUnicode_DecodeFSDefault(selectArgs.c_str()));
                return Php::Value(pyResult);*/
                //return cppSelect;
            } else {
                throw Php::Exception("Failed to import required Python modules");
            }
        } else {
            throw Php::Exception("Both token and url are required.");
        }
    }

    Php::Value select(Php::Parameters &params) {
        const std::string& token = params[0];
        std::string cppSelect = "select";
        //PyObject* deepClientSelect = PyObject_GetAttrString(deepClient, cppSelect.c_str());
        //return PyObject_CallObject(deepClientSelect, PyUnicode_DecodeFSDefault(token.c_str()));
        return 1;
    }

    Php::Value insert(Php::Parameters &params) {
        return call_python_function("insert", params[0]);
    }

    Php::Value call_python_function(const std::string& function_name, const Php::Value& query) {
        Php::Value result;
        if (deepClientModule && deepClientClass && deepClient) {
            PyObject* pyFunc = PyObject_GetAttrString(deepClient, function_name.c_str());
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
    PyObject* deepClient = nullptr;
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

        deepClientPhpWrapper.method<&DeepClientPhpWrapper::getPyObject>("getPyObject", {
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