#include "record.hpp"

Record::Record(QObject *parent)
: QSettings("xylosper.net", "CMPlayer", parent) {
}
