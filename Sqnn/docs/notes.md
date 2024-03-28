how to operate the array in the multilayer setup:

    auto kdeRoot = mlKDE->getKDE();
    auto kdeElement = &kdeRoot[1]; 
    std::cout << (*kdeElement)->QueryNewPoint({2.0}) << "\n";

    unsigned int currentPosition = kdeElement - kdeRoot;
    unsigned int nextPosition = currentPosition * 2;
    kdeElement = kdeElement + nextPosition - 1;