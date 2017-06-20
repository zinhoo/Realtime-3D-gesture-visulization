#include <math.h>
#include <GL/glew.h>
#include <vector>
#include <string>
#include <fstream>
#include "vector3d.h"
#include "myTexture.h"
#include <cstdio>
#define PI 3.14159265


using namespace std;

class myObject3D
{
public:
	GLuint buffers[6];
	glm::mat4 model_matrix;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> textures;
	myTexture teximage;
	
	myObject3D() {

		model_matrix = glm::mat4(1.0f);
	}

	void clear() {
	}
 
	void readMesh(char *filename)
	{
		string s, t, tmp;
		float x, y, z;
		//myVertex * read_vertex;
		int v, f, e;
		int total_faces, total_vertices;
		//std::map<int, std::vector<myHalfedge *>> halfedges_by_source;

		ifstream fin(filename);
		if (!fin.is_open()) cout << "Unable to open file!\n";

		// Get total amount of vertices and faces
		getline(fin, s);
		stringstream myline(s);
		myline >> t;
		total_vertices = stoi(t);
		myline >> t;
		total_faces = stoi(t);

		std::cout << total_faces << " " << total_vertices << std::endl;
		/*
		// Create vertices
		for (v = 0; v < total_vertices; v++)
		{
			read_vertex = new myVertex();
			vertices.push_back(read_vertex);
		}
		*/
		v = 0;
		f = 0;
		e = 0;
		while (getline(fin, s))
		{
			stringstream myline(s);
			myline >> t;
			if (t == "v")
			{
				myline >> tmp;
				//std::cout << "(" << stof(tmp.substr(0, tmp.find("/")));
				x = stof(tmp.substr(0, tmp.find("/")));

				myline >> tmp;
				//std::cout << ", " << stof(tmp.substr(0, tmp.find("/")));
				y = stof(tmp.substr(0, tmp.find("/")));

				myline >> tmp;
				//std::cout << ", " << stof(tmp.substr(0, tmp.find("/"))) << ")\n";
				z = stof(tmp.substr(0, tmp.find("/")));

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
				//vertices[v]->point = new myPoint3D(x, y, z);
				v++;
			}
			if (t == "f")
			{
				// Store vertices numbers in a temporal array and create half edges
				while (myline >> tmp)
					indices.push_back(stoi(tmp.substr(0, tmp.find("/"))) - 1);
				f++;
			}
		}
	}

	void computeNormal(int v1, int v2, int v3, float & x, float & y, float & z)
	{
		double dx1 = vertices[v2 * 3] - vertices[v1 * 3];
		double dx2 = vertices[v3 * 3] - vertices[v2 * 3];
		double dy1 = vertices[v2 * 3 + 1] - vertices[v1 * 3 + 1];
		double dy2 = vertices[v3 * 3 + 1] - vertices[v2 * 3 + 1];
		double dz1 = vertices[v2 * 3 + 2] - vertices[v1 * 3 + 2];
		double dz2 = vertices[v3 * 3 + 2] - vertices[v2 * 3 + 2];


		double dx = dy1 * dz2 - dz1 * dy2;
		double dy = dz1 * dx2 - dx1 * dz2;
		double dz = dx1 * dy2 - dy1 * dx2;

		double length = sqrt(dx*dx + dy*dy + dz*dz);

		x = dx / length;
		y = dy / length;
		z = dz / length;
	}
	/*************************************************************/
	void translate(double x, double y, double z)
	{
		glm::mat4 tmp = glm::translate(glm::vec3(x, y, z));
		model_matrix = tmp * model_matrix;
	}
	void rotate(double axis_x, double axis_y, double axis_z, double angle)
	{
		glm::mat4 tmp = glm::rotate((float)angle, glm::vec3(axis_x, axis_y, axis_z));
		model_matrix = tmp * model_matrix;
	}
	/*****************************************************************/
	
	void computeCylinderTexture()
	{
		int n = vertices.size()/3;
		textures.clear();
//		textures.resize(2 * n);
		GLfloat x, y, z;

		std::cout << "empieza: " << endl;

		for (int i = 0; i< n; i++)
		{
			x = vertices.at(3 * i); y = vertices.at(3 * i + 1); z = vertices.at(3 * i + 2);

			std::cout << z << endl;
			//textures[2 * i] = z;
			textures.push_back(z);
			//if (y >= 0.0f)     textures.at(2 * i + 1) = atan2(y, x) / (PI);
			//else if (y<0.0f)  textures.at(2 * i + 1) = (-atan2(y, x)) / (PI);
			if (y >= 0.0f)    
				textures.push_back(atan2(y, x) / (PI));
			else if (y<0.0f) 
				textures.push_back( (-atan2(y, x)) / (PI));

			//this has problems at the seam, when 1->0 and so interpoltion results in the whole image squeezed between the two border vertices.
			//if ( y>=0.0f )     textures[2*i+1] = atan2(  y,  x ) / (2*PI) ;
			//else if ( y<0.0f )  textures[2*i+1] = (2*PI + atan2(  y,  x )) / (2*PI) ;
		}
	}

	/*******************************************************************************/
	void computeSphereTexture()
	{
		int n = vertices.size() / 3;
		textures.clear();

		float radius = 0.6;

		//		textures.resize(2 * n);
		GLfloat x, y, z;
	
		for (int i = 0; i< n; i++)
		{
			x = vertices.at(3 * i); y = vertices.at(3 * i + 1); z = vertices.at(3 * i + 2);
	
			//textures[2 * i] = z;
			textures.push_back(acos(z / radius) / PI);
			//if (y >= 0.0f)     textures.at(2 * i + 1) = atan2(y, x) / (PI);
			//else if (y<0.0f)  textures.at(2 * i + 1) = (-atan2(y, x)) / (PI);
			if (y >= 0.0f)
				textures.push_back(acos(x / (radius * sin(PI*(textures.at(2*i)))) ) / (2 * PI));
			else if (y<0.0f)
				textures.push_back(PI + acos(x / (radius * sin(PI*(textures.at(2 * i))))) / (2 * PI));

			//this has problems at the seam, when 1->0 and so interpoltion results in the whole image squeezed between the two border vertices.
			//if ( y>=0.0f )     textures[2*i+1] = atan2(  y,  x ) / (2*PI) ;
			//else if ( y<0.0f )  textures[2*i+1] = (2*PI + atan2(  y,  x )) / (2*PI) ;
		}
	}


	void calculateMinMax(int * x_min, int * y_min, int * x_max, int * y_max)
	{
		for (int i = 0; i < vertices.size()/3; i++)
		{
			// Check for X min value of vertex
			if (vertices.at(i * 3) < vertices.at(*x_min))
				*x_min = i * 3;
			// Check for X max value of vertex
			if (vertices.at(i * 3) > vertices.at(*x_max))
				*x_max = i * 3;

			// Check for Y min value of vertex
			if (vertices.at(i * 3 + 1) < vertices.at(*y_min))
				*y_min = i * 3 + 1;
			// Check for Y max value of vertex
			if (vertices.at(i * 3 + 1) > vertices.at(*y_max))
				*y_max = i * 3 + 1;
		}
	}

	/*******************************************************************************/
	void computePlateTexture()
	{
		int n = vertices.size() / 3;
		textures.clear();

		int x_min = 0, y_min = 0, x_max = 0, y_max = 0;
		calculateMinMax(&x_min, &y_min, &x_max, &y_max);

		GLfloat x, y;

		for (int i = 0; i< n; i++)
		{
			x = vertices.at(3 * i); y = vertices.at(3 * i + 1);

			// Calculate u
			textures.push_back((x - vertices.at(x_min)) / (vertices.at(x_max) - vertices.at(x_min)));
			// Calculate v
			textures.push_back((y - vertices.at(y_min)) / (vertices.at(y_max) - vertices.at(y_min)));
		}
	}


	/*****************************************************************/
	void computeNormals()
	{
		int i, j, k;
		float x1, y1, z1;

		int n = vertices.size() / 3;
		int m = indices.size() / 3;

		//normals = new GLfloat[3 * n];

		int *incidences = new int[n];
		for (i = 0; i<3 * n; i++) normals.push_back(0.0);
		
		for (i = 0; i<n; i++) incidences[i] = 0;

		for (j = 0; j<m; j++)
		{
			computeNormal(indices[3 * j], indices[3 * j + 1], indices[3 * j + 2], x1, y1, z1);
			normals[3 * indices[3 * j]] += x1; normals[3 * indices[3 * j] + 1] += y1; normals[3 * indices[3 * j] + 2] += z1;
			normals[3 * indices[3 * j + 1]] += x1; normals[3 * indices[3 * j + 1] + 1] += y1; normals[3 * indices[3 * j + 1] + 2] += z1;
			normals[3 * indices[3 * j + 2]] += x1; normals[3 * indices[3 * j + 2] + 1] += y1; normals[3 * indices[3 * j + 2] + 2] += z1;
			incidences[indices[3 * j]]++; incidences[indices[3 * j + 1]]++; incidences[indices[3 * j + 2]]++;
		}
		for (i = 0; i<n; i++)
		if (incidences[i] != 0)
		{
			normals[3 * i] /= incidences[i]; normals[3 * i + 1] /= incidences[i]; normals[3 * i + 2] /= incidences[i];
		}

	}
	/*****************************************************************************/
	void createObjectBuffers()
	{
		glGenBuffers(4, buffers);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * 4, &vertices.front(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, &indices.front(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * 4, &normals.front(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
		glBufferData(GL_ARRAY_BUFFER, textures.size() * 4, &textures.front(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	}

	void displayObject(GLuint shaderprogram, glm::mat4 viewmatrix)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		/***********************************************/
		glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		/***********************************************/
		glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mymodel_matrix"), 1,
			GL_FALSE, &model_matrix[0][0]);
		glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(viewmatrix*model_matrix)));
		glUniformMatrix3fv(glGetUniformLocation(shaderprogram, "mynormal_matrix"), 1,
			GL_FALSE, &normal_matrix[0][0]);
		/***********************************************/
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, teximage.texName);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);// transfer layout ID = 2 to shader BUFFER[3] stored the textures
		/**********************************************************/
		/**************************************/
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}

};
