#include "DashTieManager.hpp"

int main(void) {
    auto manager = new DashTieManager();

    uint32_t val_a = 5;

    const uint32_t base = 1000;

    manager->tieDashboard(&val_a, base, "main", "val_a");
}
