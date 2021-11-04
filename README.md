# qt-qml-easy-model

This simple Qt-QML example shows how to use a utiliy class to easily create a C++ Model used in QML

![sreenshot](/screenshot.png)

## Usage

Just re-use the utility class qobject_list_model.h in your project.

If you need to declare a list of QObject-based class, here is the macro to use:

```
// Imagine a class named "Device"
DECLARE_Q_OBJECT_LIST_MODEL( Device )
```

It will create a QList based utility class named `OLM_Device`. Export it to used 

Original code : koldoon/qt-listview-generic-cpp-modelPublic

