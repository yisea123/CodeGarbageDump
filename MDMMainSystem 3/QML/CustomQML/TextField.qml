import QtQuick.Controls.Material 2.13
import QtQuick.Controls 2.13 as Quick

Quick.TextField {
    id: control
    onTextChanged:
    {
        if(!control.acceptableInput && control.text.length != 0)
            control.text = "0";
    }
}
