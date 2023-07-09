#ifndef DATASTRUCTS_HPP
#define DATASTRUCTS_HPP

struct VertexData 
{
    std::vector<float> vertexAttributes;
    unsigned int attributesPerVertex;

    // Constructors
    VertexData() : attributesPerVertex(0) {}
    VertexData(const std::vector<float>& attributes, unsigned int perVertex) : vertexAttributes(attributes), attributesPerVertex(perVertex) {}

    bool operator==(const VertexData& other) const
    {
        return vertexAttributes == other.vertexAttributes &&
            attributesPerVertex == other.attributesPerVertex;
    }

    bool operator!=(const VertexData& other) const
    {
        return !(*this == other);
    }
};

#endif // DATASTRUCTS_HPP