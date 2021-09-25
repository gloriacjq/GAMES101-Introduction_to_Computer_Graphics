#include <iostream>
#include "global.hpp"
#include "model.hpp"
#include "vector.hpp"

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Usage:\n ./MeshSimplify [Input Model File] [Output Model File] [Simplify Rate]");
        return 0;
    }
    std::string inputModelFile(argv[1]);
    std::string outputModelFile(argv[2]);
    double simplifyRate = atof(argv[3]);

    printf("inputModelFile: %s\n", inputModelFile.c_str());
    printf("outputModelFile: %s\n", outputModelFile.c_str());
    printf("simplifyRate: %.4lf\n", simplifyRate);
    printf("------------------------------------\n");

    Model model;
    model.loadModelFile(inputModelFile);
    size_t target = model.getFaceN() * simplifyRate;
    model.simplify(target);
    model.saveModelFile(outputModelFile);

    printf("Simplified model is saved to [%s].\n", outputModelFile.c_str());
    return 0;
}
