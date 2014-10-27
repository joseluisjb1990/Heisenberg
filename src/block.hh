#include <string>
#include <iostream>
#include <vector>
#include "quad.hh"
class Block
{
  private:
    std::vector<Quad>   quadList;
    std::vector<Block>  entryBlocks;
    std::vector<Block>  exitBlocks;
}
