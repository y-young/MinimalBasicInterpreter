#include "utils.h"

QString indent(QString content) {
    return content.replace('\n', "\n    ");
}
