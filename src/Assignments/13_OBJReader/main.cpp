#include "app.h"

int main() {
    SimpleShapeApplication app(1080, 800, PROJECT_NAME, true, 1);
    app.run(1);
    return 0;
}