EBO::EBO(std::vector<unsigned int>& indices)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    
    // Verify indices are within bounds of vertex count
    unsigned int maxIndex = 0;
    for (const auto& index : indices) {
        maxIndex = std::max(maxIndex, index);
    }
    
    LOG_DEBUG("Creating index buffer with %zu indices, max index: %u", 
              indices.size(), maxIndex);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 indices.size() * sizeof(unsigned int), 
                 indices.data(), 
                 GL_STATIC_DRAW);
    
    // Verify buffer creation
    GLint bufferSize = 0;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
    if (bufferSize != static_cast<GLint>(indices.size() * sizeof(unsigned int))) {
        LOG_ERROR("Failed to create index buffer. Expected size: %zu, Got: %d",
                 indices.size() * sizeof(unsigned int), bufferSize);
    }
}