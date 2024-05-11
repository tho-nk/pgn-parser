#include "piece/include/EmptyPiece.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool EmptyPiece::IsValidBasicMove_(const Position &position) const { return true; }

} // namespace mlp_ha
