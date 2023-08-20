#include "phpcpp.h"
#include <string>
#include <Python.h>

class DeepClientPhpWrapper : public Php::Base {

private:
    std::string token = "";
    std::string url = "";
    PyObject* deepClientModule = nullptr;

    void initializePython() {
        Py_Initialize();
        PyRun_SimpleString("import sys\n"
                            "sys.path.append('.');");
        deepClientModule = PyImport_ImportModule("deep_client_interface");
        if (!deepClientModule) {
            PyErr_Print();
            throw Php::Exception("Failed to import required Python modules");
        }
    }

    void finalizePython() {
        Py_XDECREF(deepClientModule);
        Py_Finalize();
    }

public:
    DeepClientPhpWrapper() {
        initializePython();
    }

    ~DeepClientPhpWrapper() {
        finalizePython();
    }

    void __construct(Php::Parameters &params) {
        if (params.size() < 2 || params[0].isNull() || params[1].isNull()) {
            throw Php::Exception("Both token and url are required.");
        }
        token = params[0].stringValue();
        url = params[1].stringValue();
    }

    Php::Value select(Php::Parameters &params) {
        return call_python_function("select", params[0]);
    }

    Php::Value insert(Php::Parameters &params) {
        return call_python_function("insert", params[0]);
    }

    Php::Value update(Php::Parameters &params) {
        return call_python_function("update", params[0]);
    }

    Php::Value deleteFunc(Php::Parameters &params) {
        return call_python_function("delete", params[0]);
    }

    Php::Value serial(Php::Parameters &params) {
        return call_python_function("serial", params[0]);
    }

    Php::Value id(Php::Parameters &params) {
        return call_python_function("id", params[0]);
    }

    Php::Value call_python_function(const std::string& function_name, const Php::Value& query) {
        Php::Value result;
        if (deepClientModule) {
            PyObject* pyFunc = PyObject_GetAttrString(deepClientModule, function_name.c_str());
            if (pyFunc && PyCallable_Check(pyFunc)) {
                PyObject* pyArgs = PyTuple_Pack(1, PyUnicode_DecodeFSDefault(query.stringValue().c_str()));
                PyObject* pyResult = PyObject_CallObject(pyFunc, pyArgs);
                if (pyResult) {
                    //result = Php::Value(PyUnicode_AsUTF8(pyResult));
                    if (PyLong_Check(pyResult)) {
                        long int_value = PyLong_AsLong(pyResult);
                        return int_value;
                    } else if (PyFloat_Check(pyResult)) {
                        double float_value = PyFloat_AsDouble(pyResult);
                        return float_value;
                    } else if (PyUnicode_Check(pyResult)) {
                        const char* str_value = PyUnicode_AsUTF8(pyResult);
                        return str_value;
                    } else if (PyList_Check(pyResult)) {
                        return "List";
                    } else if (PyTuple_Check(pyResult)) {
                        return "Tuple";
                    } else if (PyDict_Check(pyResult)) {
                        return "Dict";
                    } else {
                        throw Php::Exception("Runtime error, this type not implemented");
                    }
                    Py_DECREF(pyResult);
                } else {
                    PyErr_Print();
                    throw Php::Exception("Runtime error");
                }
                Py_DECREF(pyArgs);
            } else {
                PyErr_Print();
                throw Php::Exception("Failed to load required Python modules");
            }
            Py_XDECREF(pyFunc);
        } else {
            throw Php::Exception("Failed to import required Python modules");
        }
        return result;
    }
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
        deepClientPhpWrapper.method<&DeepClientPhpWrapper::update>("update", {
            Php::ByVal("query", Php::Type::String)
        });
        deepClientPhpWrapper.method<&DeepClientPhpWrapper::deleteFunc>("delete", {
            Php::ByVal("query", Php::Type::String)
        });
        deepClientPhpWrapper.method<&DeepClientPhpWrapper::serial>("serial", {
            Php::ByVal("query", Php::Type::String)
        });
        deepClientPhpWrapper.method<&DeepClientPhpWrapper::id>("id", {
            Php::ByVal("query", Php::Type::String)
        });
        extension.add(std::move(deepClientPhpWrapper));

        return extension;
    }
}