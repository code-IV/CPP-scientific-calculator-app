QT += core gui widgets qml
CONFIG += c++17

# Set output directories
DESTDIR = build                  # Final executable location
OBJECTS_DIR = build/obj          # Object files directory
MOC_DIR = build/moc              # MOC-generated files directory
UI_DIR = build/ui                # UI compiler output (future use)
RCC_DIR = build/rcc              # Resource compiler output (future use)

# Source files
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

# Header files
HEADERS += \
    src/headers/mainwindow.h     # Adjusted to match your headers path

# For QML projects (if needed)
# RESOURCES += qml.qrc

# Disable deprecated APIs (optional)
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# Additional compiler flags
QMAKE_CXXFLAGS += -Wall -Wextra

# Deployment rules (simplified)
target.path = $$DESTDIR
INSTALLS += target