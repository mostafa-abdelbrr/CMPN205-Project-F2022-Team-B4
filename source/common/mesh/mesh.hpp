#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
        GLsizei VertxCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            //TODO: (Req 2) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            elementCount=elements.size();

            //Generate Vertex Array and Bind inorder to use it
            glGenVertexArrays(1,&VAO);
            glBindVertexArray(VAO);

            //Create Vertex Buffer and bind  inorder to fill it
            glGenBuffers(1,&VBO);
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            // Now lets fill Buffer
            VertxCount=vertices.size();
            glBufferData(GL_ARRAY_BUFFER,VertxCount*sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);

            //Create ELement Buffer
            glGenBuffers(1,&EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,elementCount*sizeof(unsigned int),&elements[0],GL_STATIC_DRAW);
             // Now utilizing the Vertex array to work on Buffers

             //Attributes 

             //position Attribute
             glEnableVertexAttribArray(ATTRIB_LOC_POSITION); //Enable it
             //abd2 22ra mn Position
             glVertexAttribPointer(ATTRIB_LOC_POSITION,3,GL_FLOAT,false,sizeof(Vertex),(void*)offsetof(Vertex,position));

             // Apply on other attributes
             glEnableVertexAttribArray(ATTRIB_LOC_COLOR);

             glVertexAttribPointer(ATTRIB_LOC_COLOR,4,GL_UNSIGNED_BYTE,true,sizeof(Vertex),(void*)offsetof(Vertex,color));



             glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);

             glVertexAttribPointer(ATTRIB_LOC_NORMAL,3,GL_FLOAT,false,sizeof(Vertex), (void*)offsetof(Vertex, normal));

             
             glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);

             glVertexAttribPointer(ATTRIB_LOC_TEXCOORD,2,GL_FLOAT,false,sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));

             
             


          




            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
            
        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, elementCount,GL_UNSIGNED_INT,(void*)0);

        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: (Req 2) Write this function
            glDeleteVertexArrays(1,&VBO);
            glDeleteVertexArrays(1,&VAO);
            glDeleteVertexArrays(1,&EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}