#pragma once
#include "TStd.h"

namespace AStar
{
	using uint = unsigned int;
	using HeuristicFunction = std::function<uint(TIndex, TIndex)>;
	using CoordinateList = std::vector<TIndex>;

	struct Node
	{
		uint G, H;
		TIndex coordinates;
		Node *parent;

		Node(TIndex coord_, Node *parent_ = nullptr);
		uint getScore();
	};

	using NodeSet = std::set<Node*>;

	class Generator
	{
		bool detectCollision(TIndex coordinates_);
		Node* findNodeOnList(NodeSet& nodes_, TIndex coordinates_);
		void releaseNodes(NodeSet& nodes_);

	public:
		Generator();
		void setWorldSize(TIndex worldSize_);
		void setDiagonalMovement(bool enable_);
		void setHeuristic(HeuristicFunction heuristic_);
		CoordinateList findPath(TIndex source_, TIndex target_);
		void addCollision(TIndex coordinates_);
		void removeCollision(TIndex coordinates_);
		void clearCollisions();

	private:
		HeuristicFunction heuristic;
		CoordinateList direction, walls;
		TIndex worldSize;
		uint directions;
	};

	class Heuristic
	{
		static TIndex getDelta(TIndex source_, TIndex target_);

	public:
		static uint manhattan(TIndex source_, TIndex target_);//맨헤튼
		static uint euclidean(TIndex source_, TIndex target_);//유클리드
		static uint octagonal(TIndex source_, TIndex target_);//8변
	};
}

