
#ifndef CRYSTALHELPERS
#define CRYSTALHELPERS

#include <Nepomuk/Resource>

//using namespace Crystal
namespace Crystal {
// For sorting results
bool ratingLessThan(const Nepomuk::Resource *r1, const Nepomuk::Resource *r2)
{
    return r1->rating() < r2->rating();
}
}
#endif