// Load data from .obj file and print it into a file called output.txt
// Output.txt is a file to see the data that was loaded from the .obj file

// STD Libs
#include <iostream>
#include <fstream>
#include <memory>
//#include <windows.h>

// OBJ_Loader
#include "OBJ_Loader.h"

// My Libs
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "primitives/plane.h"
#include "primitives/sphere.h"
#include "primitives/cube.h"
#include "portal.h"
#include "utils.h"
#include "vec3.h"

void printLoadedFile(objl::Loader Loader, std::ofstream& file) {
	// Go through each loaded mesh and out its contents
	for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
	{
		// Copy one of the loaded meshes to be our current mesh
		objl::Mesh curMesh = Loader.LoadedMeshes[i];

		// Print Mesh Name
		file << "Mesh " << i << ": " << curMesh.MeshName << "\n";

		// Print Vertices
		file << "Vertices:\n";

		// Go through each vertex and print its number,
		//  position, normal, and texture coordinate
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			file << "V" << j << ": " <<
				"P(" << curMesh.Vertices[j].Position.X << ", " << curMesh.Vertices[j].Position.Y << ", " << curMesh.Vertices[j].Position.Z << ") " <<
				"N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
				"TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";
		}

		// Print Indices
		file << "Indices:\n";

		// Go through every 3rd index and print the
		//	triangle that these indices represent
		for (int j = 0; j < curMesh.Indices.size(); j += 3)
		{
			file << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";
		}

		// Print Material - commented parameters are not changing in sample scene
		file << "Material: " << curMesh.MeshMaterial.name << "\n";
		//file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
		file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
		//file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
		file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
		file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
		//file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
		//file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
		//file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
		//file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
		//file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
		//file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
		//file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";

		// Leave a space to separate from the next mesh
		file << "\n";

		/*
		Example output:
			Mesh 5: Plane.004
			Vertices:
			V0: P(-1.99999, 3, 2) N(1, -0, -0) TC(0, 0)
			V1: P(-2.00001, -1, 2) N(1, -0, -0) TC(1, 0)
			V2: P(-2.00001, -1, -2) N(1, -0, -0) TC(1, 1)
			V3: P(-1.99999, 3, -2) N(1, -0, -0) TC(0, 1)
			Indices:
			T0: 0, 1, 3
			T1: 1, 2, 3
			Material: red
			Diffuse Color: 1, 0, 0
			Specular Exponent: 250
			Optical Density: 1.45

			Mesh 6: Plane.005
			Vertices:
			V0: P(-0.5, 3, 0.5) N(-0, 1, -0) TC(0, 0)
			V1: P(0.5, 3, 0.5) N(-0, 1, -0) TC(1, 0)
			V2: P(0.5, 3, -0.5) N(-0, 1, -0) TC(1, 1)
			V3: P(-0.5, 3, -0.5) N(-0, 1, -0) TC(0, 1)
			Indices:
			T0: 0, 1, 3
			T1: 1, 2, 3
			Material: light
			Diffuse Color: 0.8, 0.8, 0.8
			Specular Exponent: 360
			Optical Density: 1
		*/
	}
}

hittable_list saveLoadedSceneAsPrimitives(objl::Loader Loader) {
	// Go through each loaded mesh and save it as an instance of a primitive
	hittable_list world;
	point3 entryPortalVertices1[3];
	point3 exitPortalVertices1[3];
	point3 entryPortalVertices2[3];
	point3 exitPortalVertices2[3];
	for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
		objl::Mesh curMesh = Loader.LoadedMeshes[i];
		// set material
		shared_ptr<material> material;
		if (curMesh.MeshMaterial.Ns == 360) {
			int x = 20;
			material = make_shared<diffuse_light>(color(curMesh.MeshMaterial.Kd.X * x, curMesh.MeshMaterial.Kd.Y * x, curMesh.MeshMaterial.Kd.Z * x));
		} else {
			material = make_shared<lambertian>(color(curMesh.MeshMaterial.Kd.X, curMesh.MeshMaterial.Kd.Y, curMesh.MeshMaterial.Kd.Z));
		}
		
		// save as primitive
		if (curMesh.MeshName.find("Sphere") != std::string::npos) {
			// find center of sphere
			int minY = std::numeric_limits<int>::max();
			int maxY = std::numeric_limits<int>::min();
			int minX = std::numeric_limits<int>::max();
			int maxX = std::numeric_limits<int>::min();
			int minZ = std::numeric_limits<int>::max();
			int maxZ = std::numeric_limits<int>::min();
			for (int i = 0; i < curMesh.Vertices.size(); i++) {
				if (curMesh.Vertices[i].Position.Y < minY) {
					minY = curMesh.Vertices[i].Position.Y;
				}
				if (curMesh.Vertices[i].Position.Y > maxY) {
					maxY = curMesh.Vertices[i].Position.Y;
				}
				if (curMesh.Vertices[i].Position.X < minX) {
					minX = curMesh.Vertices[i].Position.X;
				}
				if (curMesh.Vertices[i].Position.X > maxX) {
					maxX = curMesh.Vertices[i].Position.X;
				}
				if (curMesh.Vertices[i].Position.Z < minZ) {
					minZ = curMesh.Vertices[i].Position.Z;
				}
				if (curMesh.Vertices[i].Position.Z > maxZ) {
					maxZ = curMesh.Vertices[i].Position.Z;
				}
			}
			point3 center = point3((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2);
			// find radius of sphere
			double radius = (maxY - minY) / 2;
			// create sphere
			world.add(make_shared<sphere>(center, radius, material));
		}
		else if (curMesh.MeshName.find("Plane") != std::string::npos) {
			point3 planeVertices[4];
			for (int j = 0; j < curMesh.Vertices.size(); j++) {
				// Change the order from 0, 1, 2, 3 to 0, 1, 3, 2
				int newIndex = (j == 2) ? 3 : (j == 3) ? 2 : j;
				planeVertices[newIndex] = point3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);
			}

			world.add(make_shared<quad>(planeVertices, material));
		}
		else if (curMesh.MeshName.find("Cube") != std::string::npos) {
			point3 cubeVertices[8];
			// Counter for unique vertices
			int vertexCount = 0;

			for (int j = 0; j < curMesh.Vertices.size() && vertexCount < 8; j++) {

				// Create a point3 with vertex coordinates
				point3 currentVertex(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);

				// Check if the vertex is already in the array
				bool isDuplicate = false;
				for (int k = 0; k < vertexCount; k++) {
					if (cubeVertices[k] == currentVertex) {
						isDuplicate = true;
						break;
					}
				}
				// If it's not a duplicate, add it to the array
				if (!isDuplicate) {
					cubeVertices[vertexCount] = currentVertex;
					vertexCount++;
				}
			}
			world.add(make_shared<cube>(cubeVertices, material));
		} else if(curMesh.MeshName.find("Portal") != std::string::npos) {
			if (curMesh.MeshName.find("Portal_we_1") != std::string::npos) {
				for (int j = 0; j < curMesh.Vertices.size(); j++) {
					entryPortalVertices1[j] = point3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);
				}
			} else if (curMesh.MeshName.find("Portal_wy_1") != std::string::npos) {
				for (int j = 0; j < curMesh.Vertices.size(); j++) {
					exitPortalVertices1[j] = point3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);
				}
			}
			// no "!=" operator overload for point3, so I have to check if all coordinates are not 0
			if (exitPortalVertices1[0].x() != 0 && exitPortalVertices1[0].y() != 0 && exitPortalVertices1[0].z() != 0 && entryPortalVertices1[0].x() != 0 && entryPortalVertices1[0].y() != 0 && entryPortalVertices1[0].z() != 0) {
				auto portal_out_mat = make_shared<portal_out>();
				auto exit_portal = make_shared<triangle>(exitPortalVertices1[1], exitPortalVertices1[0], exitPortalVertices1[2], portal_out_mat);

				auto portal_in_mat = make_shared<portal_in>(exit_portal);
				auto entry_portal = make_shared<triangle>(entryPortalVertices1[1], entryPortalVertices1[0], entryPortalVertices1[2], portal_in_mat);

				world.add(exit_portal);
				world.add(entry_portal);
			}
			if (curMesh.MeshName.find("Portal_we_2") != std::string::npos) {
				for (int j = 0; j < curMesh.Vertices.size(); j++) {
					entryPortalVertices2[j] = point3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);
				}
			} else if (curMesh.MeshName.find("Portal_wy_2") != std::string::npos) {
				for (int j = 0; j < curMesh.Vertices.size(); j++) {
					exitPortalVertices2[j] = point3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);
				}
			}
			if (exitPortalVertices2[0].x() != 0 && exitPortalVertices2[0].y() != 0 && exitPortalVertices2[0].z() != 0 && entryPortalVertices2[0].x() != 0 && entryPortalVertices2[0].y() != 0 && entryPortalVertices2[0].z() != 0) {
				auto portal_out_mat2 = make_shared<portal_out>();
				auto exit_portal2 = make_shared<triangle>(exitPortalVertices1[1], exitPortalVertices1[0], exitPortalVertices1[2], portal_out_mat2);

				auto portal_in_mat2 = make_shared<portal_in>(exit_portal2);
				auto entry_portal2 = make_shared<triangle>(entryPortalVertices1[1], entryPortalVertices1[0], entryPortalVertices1[2], portal_in_mat2);

				world.add(exit_portal2);
				world.add(entry_portal2);
			}
		}
	}
	return world;
}

void setCamera(hittable_list world) {
	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 300;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.background = color(0, 0, 0);

	cam.vfov = 40;
	cam.lookfrom = point3(0, 2, 7);
	cam.lookat = point3(0, 1, 0);
	cam.vup = vec3(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

/*
-f input_file_name
-o output_file_name
-w image_width
-h image_height
-s samples_per_pixel
-d max_depth
*/
int main() {
	// Initialize Loader and scene world
    objl::Loader Loader;
	hittable_list world;
    // Load .obj File
    bool loadout = Loader.LoadFile("input/my_Cornell_Box_portals_x2.obj");
	// Create/Open output.txt
	std::ofstream file("output/output.txt");

	if(loadout) {
		printLoadedFile(Loader, file);
		world = saveLoadedSceneAsPrimitives(Loader);

		file.close();
	} else {
        file << "Nie udalo sie zaladowac pliku.\n";
        file.close();
    }
	setCamera(world);

	return 0;
}