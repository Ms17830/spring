/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef S3O_PARSER_H
#define S3O_PARSER_H

#include "3DModel.h"
#include "IModelParser.h"

#include "System/type2.h"

#include <map>

enum {
	S3O_PRIMTYPE_TRIANGLES      = 0,
	S3O_PRIMTYPE_TRIANGLE_STRIP = 1,
	S3O_PRIMTYPE_QUADS          = 2,
};


typedef SVertexData SS3OVertex;


struct SS3OPiece: public S3DModelPiece {
	SS3OPiece(): primType(S3O_PRIMTYPE_TRIANGLES) {
	}

public:
	void UploadGeometryVBOs();
	void DrawForList() const;

	unsigned int GetVertexDrawIndexCount() const override { return indices.size(); }
	unsigned int GetVertexCount() const override { return vertices.size(); }
	unsigned int GetNormalCount() const override { return vertices.size(); }
	unsigned int GetTxCoorCount() const override { return vertices.size(); }

	const float3& GetVertexPos(const int idx) const override { return vertices[idx].pos; }
	const float3& GetNormal(const int idx) const override { return vertices[idx].normal; }

public:
	void SetVertexCount(unsigned int n) { vertices.resize(n); }
	void SetIndexCount(unsigned int n) { indices.resize(n); }
	void SetVertex(int idx, const SS3OVertex& v) { vertices[idx] = v; } //FIXME
	void SetIndex(int idx, const unsigned int drawIdx) { indices[idx] = drawIdx; }

	void Trianglize();
	void SetMinMaxExtends();
	void SetVertexTangents();
	void Shatter(float pieceChance, int texType, int team, const float3 pos, const float3 speed, const CMatrix44f& m) const;

public:
	int primType;
	std::vector<SS3OVertex> vertices;
	std::vector<unsigned int> indices;
};



class CS3OParser: public IModelParser
{
public:
	S3DModel* Load(const std::string& name);

private:
	SS3OPiece* LoadPiece(S3DModel*, SS3OPiece*, unsigned char* buf, int offset);
};

#endif /* S3O_PARSER_H */
