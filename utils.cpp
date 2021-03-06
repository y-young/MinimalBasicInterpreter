#include "utils.h"

QString indent(QString content) {
    return content.replace('\n', "\n    ");
}

void showErrorMessage(const Exception& error) {
    QMessageBox::critical(nullptr, "Error", error.what());
}
