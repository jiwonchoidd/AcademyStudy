#include "TAStar.h"
#include <algorithm>

using namespace std::placeholders;

//std::bind(함수 주소, 함수인자1, 함수인자2, 함수인자3, ...)
//std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ... 등을 새로운 함수 인자와 맵핑 시킬수있다.
//void Func(int iA, float fB, bool bC);
//auto funcA = std::bind(Func, 10, 2.0f, true);funcA();
//auto funcB = std::bind(Func, 10, std::placeholders::_1, true);funcB(2.0f);
//auto funcC = std::bind(Func, std::placeholders::_2, std::placeholders::_1, true);funcC(2.0f, 10);
//auto funcD = std::bind(Func, std::placeholders::_2, std::placeholders::_1, std::placeholders::_3);funcD(2.0f, 10, true);
//그리고 전역함수가 아닌 클래스 함수의 경우
//&ClassA::FuncA 행태를 바인드의 첫번쩨 인자로 사용한다


AStar::Node::Node(TIndex coordinates_, Node *parent_)
{
	parent = parent_;
	coordinates = coordinates_;
	G = H = 0;
}

AStar::uint AStar::Node::getScore()
{
	return G + H;
}

AStar::Generator::Generator()
{
	setDiagonalMovement(false);
	setHeuristic(&Heuristic::manhattan);
	direction = {
		{ 0, 1 }, // 북
	{ 1, 0 }, // 동
	{ 0, -1 },// 서
	{ -1, 0 },// 남
	{ -1, -1 },{ 1, 1 },{ -1, 1 },{ 1, -1 }
	};
}

void AStar::Generator::setWorldSize(TIndex worldSize_)
{
	worldSize = worldSize_;
}

void AStar::Generator::setDiagonalMovement(bool enable_)
{
	directions = (enable_ ? 8 : 4);
}

void AStar::Generator::setHeuristic(HeuristicFunction heuristic_)
{
	heuristic = std::bind(heuristic_, _1, _2);
}

void AStar::Generator::addCollision(TIndex coordinates_)
{
	walls.push_back(coordinates_);
}

void AStar::Generator::removeCollision(TIndex coordinates_)
{
	auto it = std::find(walls.begin(), walls.end(), coordinates_);
	if (it != walls.end())
	{
		walls.erase(it);
	}
}

void AStar::Generator::clearCollisions()
{
	walls.clear();
}

AStar::CoordinateList AStar::Generator::findPath(TIndex source_, TIndex target_)
{
	Node *current = nullptr;
	NodeSet openSet, closedSet;
	openSet.insert(new Node(source_));
	vector< TIndex> currentList;
	while (!openSet.empty())
	{
		current = *openSet.begin();
		for (auto node : openSet)
		{
			int iNodeValue = node->getScore();
			int iCurrentValue = current->getScore();
			if (iNodeValue <= iCurrentValue)
			{
				current = node;
			}
		}

		if (current->coordinates == target_)
		{
			break;
		}
		currentList.push_back(current->coordinates);
		closedSet.insert(current);
		openSet.erase(std::find(openSet.begin(), openSet.end(), current));

		for (uint i = 0; i < directions; ++i)
		{
			TIndex newCoordinates(current->coordinates + direction[i]);
			if (detectCollision(newCoordinates))
			{
				continue;
			}
			if (findNodeOnList(closedSet, newCoordinates))
			{
				continue;
			}

			uint totalCost = current->G + ((i < 4) ? 10 : 14);
			//uint totalCost = current->G + ((i < 4) ? 1 : 2);

			Node *successor = findNodeOnList(openSet, newCoordinates);
			if (successor == nullptr)
			{
				successor = new Node(newCoordinates, current);
				successor->G = totalCost;
				successor->H = heuristic(successor->coordinates, target_);
				openSet.insert(successor);
			}
			else if (totalCost < successor->G)
			{
				successor->parent = current;
				successor->G = totalCost;
			}
		}
	}

	CoordinateList path;
	while (current != nullptr)
	{
		path.push_back(current->coordinates);
		current = current->parent;
	}

	releaseNodes(openSet);
	releaseNodes(closedSet);

	return path;
}

AStar::Node* AStar::Generator::findNodeOnList(NodeSet& nodes_, TIndex coordinates_)
{
	for (auto node : nodes_)
	{
		if (node->coordinates == coordinates_)
		{
			return node;
		}
	}
	return nullptr;
}

void AStar::Generator::releaseNodes(NodeSet& nodes_)
{
	for (auto it = nodes_.begin(); it != nodes_.end();)
	{
		delete *it;
		it = nodes_.erase(it);
	}
}

bool AStar::Generator::detectCollision(TIndex coordinates_)
{
	if (coordinates_.x < 0 || coordinates_.x >= worldSize.x ||
		coordinates_.y < 0 || coordinates_.y >= worldSize.y ||
		std::find(walls.begin(), walls.end(), coordinates_) != walls.end())
	{
		return true;
	}
	return false;
}

TIndex AStar::Heuristic::getDelta(TIndex source_, TIndex target_)
{
	return{ abs(source_.x - target_.x),  abs(source_.y - target_.y) };
}

AStar::uint AStar::Heuristic::manhattan(TIndex source_, TIndex target_)
{
	auto delta = std::move(getDelta(source_, target_));
	return static_cast<uint>(10 * (delta.x + delta.y));
}

AStar::uint AStar::Heuristic::euclidean(TIndex source_, TIndex target_)
{
	TIndex a = getDelta(source_, target_);
	TIndex&& delta = std::move(getDelta(source_, target_));
	return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

AStar::uint AStar::Heuristic::octagonal(TIndex source_, TIndex target_)
{
	auto delta = std::move(getDelta(source_, target_));
	return 10 * (delta.x + delta.y) + (-6) * min(delta.x, delta.y);
}