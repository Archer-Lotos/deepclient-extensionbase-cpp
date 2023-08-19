#include "phpcpp.h"
#include <string>
#include <Python.h>

class GqlPhpWrapper : public Php::Base {
public:
    GqlPhpWrapper() {
        Py_Initialize();
        pyModule = PyImport_ImportModule("gql");
        pyClientInstance = PyObject_GetAttrString(pyModule, "deep_client_interface");
    }

    ~GqlPhpWrapper() {
        Py_XDECREF(pyClientInstance);
        Py_DECREF(pyModule);
        Py_Finalize();
    }

    Php::Value select(Php::Parameters &params) {
        return call_python_function("select", params[0]);
    }

    Php::Value insert(Php::Parameters &params) {
        return call_python_function("insert", params[0]);
    }

    Php::Value call_python_function(const std::string& function_name, const Php::Value& query) {
        Php::Value result;
        if (pyModule && pyClientInstance) {
            PyObject* pyFunc = PyObject_GetAttrString(pyClientInstance, function_name.c_str());
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

    Php::Value executeQuery(const Php::Value& query) {
        Php::Value result;
        if (pyModule) {
            PyObject* pyFunc = PyObject_GetAttrString(pyModule, "deep_client_interface");
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
    PyObject* pyModule = nullptr;
    PyObject* pyClientInstance = nullptr;
};


extern "C" {
    PHPCPP_EXPORT void *get_module() {
        static Php::Extension extension("deep_client_php_extension", "1.0");

        Php::Class<GqlPhpWrapper> gqlPhpWrapper("GqlPhpWrapper");
        gqlPhpWrapper.method<&GqlPhpWrapper::select>("select", {
            Php::ByVal("query", Php::Type::String)
        });
        gqlPhpWrapper.method<&GqlPhpWrapper::insert>("insert", {
            Php::ByVal("query", Php::Type::String)
        });

        extension.add(std::move(gqlPhpWrapper));

        return extension;
    }
}