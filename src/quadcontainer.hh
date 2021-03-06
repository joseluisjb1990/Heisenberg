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
    void  addNumberBlock(unsigned int nb) { _numberBlock = nb;                          }
    void  replaceAddress(unsigned int ad) { _quad->setDestiny(std::to_string(ad));      }
    unsigned int  getNumberBlock()        { return _numberBlock;                        }
    void setLiveVar(set<string> s)        { _quad->_liveVar = s;                                        }
    bool isLiveVar(string s)              { return  _quad->_liveVar.find(s) != _quad->_liveVar.end();   }

    Quad*         _quad;
    unsigned int  _numberQuad;
    unsigned int  _numberBlock;
    bool          _isLeader;
    bool          _isDead = false;
    string        _rLeft;
    string        _rRight;
    string        _rDestiny;
};
#endif
