#ifndef RIGIDBODYFACTORY_HPP
#define RIGIDBODYFACTORY_HPP

#include <memory>
#include <optional>

#include "FactoryStructs.hpp"
#include "RigidBody.hpp"
#include "ObjectStructs.hpp"

class RigidBodyFactory
{
public:
    static RigidBodyFactory& getInstance();

    virtual std::optional<std::unique_ptr<RigidBody>> createRigidBody(const ObjectConfig& objectConfig);

protected:
    RigidBodyFactory();
};

#endif // RIGIDBODYFACTORY_HPP
