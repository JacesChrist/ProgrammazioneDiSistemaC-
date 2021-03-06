#include <iostream>

#include "SharedEditor.h"

SharedEditor::SharedEditor(NetworkServer& ns) : _server(const_cast<NetworkServer&> (ns)){
    this->setIdSharedEditor(this->_server.connect(this));
    this->_counter = 0;
    this->initCRDT();
}

SharedEditor::~SharedEditor() = default;

int SharedEditor::getIdScharedEditor() {
    return this->_siteId;
}

int SharedEditor::getCounterSharedEditor() {
    return this->_counter;
}

void SharedEditor::incrementCounterSharedEditor() {
    this->_counter++;
}

void SharedEditor::setIdSharedEditor(int id) {
    this->_siteId = id;
}

void SharedEditor::initCRDT() {
    this->_symbols.push_front(Symbol(' ',this->getIdScharedEditor(),-1,{0}));
    this->_symbols.push_back(Symbol(' ',this->getIdScharedEditor(),-1,{1}));
}

void SharedEditor::localInsert(int index,char value) {
    this->incrementCounterSharedEditor();
    //Symbol tempSymbol = Symbol(value,this->getIdScharedEditor(),this->getCounterSharedEditor(),this->calculateNewSRDT(index,this->_symbols));
    int i = 0;
    std::vector<int> newCRDT;
    Symbol newSymbol = Symbol(value, this->getIdScharedEditor(), this->getCounterSharedEditor(), newCRDT);
    if(index == 0) {
        //TODO: need minimum number after 0
        int j = 0;
        auto it = this->_symbols.begin();
        it++;
        for (auto it2 = it->positionCRDT.begin();it2 != it->positionCRDT.end();++it2) {
            newCRDT.push_back(*it2);
            j++;
        }
        j--;
        newCRDT[j]--;
        newSymbol.positionCRDT = newCRDT;
        this->_symbols.insert(it,newSymbol);
        //TODO: check
    }
    else {
      if (index == (this->_symbols.size() - 2)) {
          //TODO: need maximum number before 1
          int j = 0;
          auto it = this->_symbols.end();
          it--;
          for (auto it2 = it->positionCRDT.begin();it2 != it->positionCRDT.end();++it2) {
              newCRDT.push_back(*it2);
              j++;
          }
          j--;
          if(newCRDT[j] != 9)
            newCRDT[j]--;
          else
              newCRDT.push_back(1);
          newSymbol.positionCRDT = newCRDT;
          this->_symbols.insert(it,newSymbol);
          //TODO: check
      }
      else {
          for (auto it = this->_symbols.begin(); it != this->_symbols.end(); ++it) {
              if (i == (index - 1)) {
                  int j = 0;
                  for (auto it2 = it->positionCRDT.begin();it2 != it->positionCRDT.end();++it2) {
                      newCRDT.push_back(*it2);
                      j++;
                  }
                  j--;
                  if((it++->positionCRDT[j] - it->getCRDTSymbol()[j]) >= 1) //TODO: check it++-> (?)
                      newCRDT[j]++;
                  else
                      newCRDT.push_back(1);
                  newSymbol.positionCRDT = newCRDT;
                  this->_symbols.insert(it,newSymbol);
                  break;
              }
              i++;
          }
      }
    }
    this->_server.getMessageVector().push_back(Message(newSymbol,true));
}

void SharedEditor::localErase(int index) {
    int i = 0;
    for(auto it = this->_symbols.begin();it != this->_symbols.end();++it) {
        if (i == index) {
            this->_symbols.erase(it);
            break;
        }
    }
    this->_server.getMessageVector().push_back(Message(Symbol(' ',-1,-1,std::vector<int> {}),false));
}

std::string SharedEditor::to_string() {
    std::string output;
    for(auto it = this->_symbols.begin();it != this->_symbols.end();++it) {
        output.append(&it->symbol);
    }
    std::cout<<output;
}

void SharedEditor::process(const Message& m) {
    if(m.isInsert) {
        for(auto it = this->_symbols.begin(); it != this->_symbols.end(); ++it) {
            try {
                if(it->positionCRDT == m.symbolCRDT) {
                    throw 0 ;
                }
                if(it->positionCRDT > m.symbolCRDT) {
                    --it;
                    this->_symbols.insert(it,m.symbol);
                }
            }
            catch(int a) {
                std::cout<<"collision";
                exit(-1);
            }
        }
    }
    else {
        for(auto it = this->_symbols.begin(); it != this->_symbols.end(); ++it) {
            if(it->positionCRDT == m.symbolCRDT)
                this->_symbols.erase(it);
        }
    }
}
