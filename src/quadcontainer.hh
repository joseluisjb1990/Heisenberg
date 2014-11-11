#ifndef QUADCONTAINER_HH
#define QUADCONTAINER_HH

#include "quad.hh"

class QuadContainer
{
  public:
    QuadContainer (Quad* quad, unsigned int numberQuad, bool isLeader);
    void  print       ();
    Quad* getQuad     () { return _quad;                }
    void  setLeader   () { _isLeader = true;            }
    bool  isLeader    () { return _isLeader;            }
    bool  isJump      () { return _quad->isJump();      }
    int   getAddress  () { return _quad->getAddress();  }
    bool  isTag       () { return _quad->isTag();       }
    void  addNumberBlock(unsigned int nb) { _numberBlock = nb;                      }
    void  replaceAddress(unsigned int ad) { _quad->setDestiny(std::to_string(ad));  }
    unsigned int  getNumberBlock()        { return _numberBlock;                    }
 
    Quad*         _quad;
    unsigned int  _numberQuad;
    unsigned int  _numberBlock;
    bool          _isLeader;
    bool          _isDead = false;
};
#endif
