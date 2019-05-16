#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>

#include "Directory.h"

std::shared_ptr<Directory> Directory::root;

Directory::Directory() = default;

//Directory::~Directory() = default;

//getter

std::shared_ptr<Directory> Directory::getSelfPointer() {
    return this->selfPointer.lock();
}

std::shared_ptr<Directory> Directory::getFatherPointer() {
    return this->fatherPointer.lock();
}

//setter

void Directory::setSelfPointer(std::weak_ptr<Directory> weakPtr) {
    this->selfPointer = weakPtr;
    return;
}

void Directory::setFatherPointer(std::weak_ptr<Directory> weakPtr) {
    this->fatherPointer = weakPtr;
    return;
}

//miscellaneous

Directory &Directory::operator=(Directory *d) {
    return *this;
}

/*int Directory::mType() const {
    return 0;
}*/

Directory::Directory(std::string name) {
    this->name = std::move(name);
}

//

std::shared_ptr<Directory> Directory::getRoot() {
    if(!root) {
        Directory::root =  std::shared_ptr<Directory>(new Directory("/"));
        Directory::root.get()->setSelfPointer(Directory::root);
        Directory::root.get()->setFatherPointer(Directory::root);
        std::cout<<"Root creata\n";
    }
    return root;
}

std::shared_ptr<Directory> Directory::addDirectory (std::string nome) {
    std::shared_ptr<Directory> newDirectoryPtr = std::shared_ptr<Directory>(new Directory(nome));
    newDirectoryPtr->setSelfPointer(newDirectoryPtr);
    newDirectoryPtr->setFatherPointer(this->getSelfPointer());
    this->innerPointers.insert({nome,newDirectoryPtr});
    std::cout<<"aggiunta directory "<<nome<<"\n";
    return newDirectoryPtr;
}

std::shared_ptr<File> Directory::addFile (std::string nome, uintmax_t size) {
    std::shared_ptr<File> newFilePtr = std::shared_ptr<File>(new File(nome,size));\
    this->innerPointers.insert({nome,newFilePtr});
    std::cout<<"aggiunto file "<<nome<<"\n";
    return newFilePtr;
}

std::shared_ptr<Base> Directory::get (std::string nome) {
    if (nome.compare(("..")))
        return this->getFatherPointer();
    if (nome.compare("."))
        return this->getSelfPointer();
    try {
        std::shared_ptr<Base> search = this->innerPointers.find(nome)->second;
        if(search != this->innerPointers.end()->second)
            return search;
    }
}