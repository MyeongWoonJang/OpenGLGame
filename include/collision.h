#ifndef _collision
#define _collision

#include "AABB.h"
#include "OBB.h"
#include <vector>

class ComponentCollide
{
public:
	template < typename T >
	using ContBB = std::vector< T >;
	template < typename T >
	using ContNode = std::vector< T >;

	static bool will_get_desc;

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';
			ss << fore_indent << indent << "AABB count: " << aabbs.size() << '\n';
			ss << fore_indent << indent << "OBB count: " << obbs.size() << '\n';

			ss << fore_indent << indent << "AABBs: " << '\n';
			for ( const auto& aabb : aabbs )
			{
				ss << aabb.get_desc( fore_indent + indent ).rdbuf();
			}

			ss << fore_indent << indent << "OBBs: " << '\n';
			for ( const auto& obb : obbs )
			{
				ss << obb.get_desc( fore_indent + indent ).rdbuf();
			}

			ss << fore_indent << indent << "children: " << children.size() << '\n';
			for ( const auto& child : children )
			{
				ss << child.get_desc( fore_indent + indent ).rdbuf();
			}
		}

		return ss;
	}

	void add_child( const ComponentCollide& child ) { children.push_back( child ); }
	void add_child( ComponentCollide&& child ) { children.push_back( std::move( child ) ); }
	void push_aabb( const CCoord& base, const glm::vec3& initial_half_len, const glm::vec3& initial_movement = glm::vec3{ 0.f, 0.f, 0.f } )
	{
		AABB aabb;
		aabb.init( base, initial_half_len, initial_movement );
		aabbs.push_back( std::move( aabb ) );
	}
	void push_obb( const CCoord& base, const glm::vec3& initial_half_len, const glm::vec3& initial_movement = glm::vec3{ 0.f, 0.f, 0.f } )
	{
		OBB obb;
		obb.init( base, initial_half_len, initial_movement );
		obbs.push_back( std::move( obb ) );
	}

	void clear()
	{
		aabbs.clear();
		obbs.clear();
		for ( auto& child : children )
		{
			child.clear();
		}
	}

	const bool collide( const ComponentCollide& other ) const
	{
		// ��Ʈ �������� �˻�
		if ( level_collide( other ) )
		{
			// ���� ����: �� �ʿ� ���̻� �ڽ��� ���� ��
			if ( children.empty() && other.children.empty() ) return true;

			for ( const auto& child : children )
			{
				// �� �ڽİ� ��밡 �浹���� �ʴ´ٸ� false
				if ( !child.collide( other ) ) return false;
			}

			for ( const auto& ochild : other.children )
			{
				// ��� �ڽİ� ���� �浹���� �ʴ´ٸ� false
				if ( !ochild.collide( *this ) ) return false;
			}
		}
		else return false;		// ��Ʈ ������������ �浹�� �ƴϸ� �浹�� �ƴ� ��
	}

private:
	ContBB< AABB > aabbs;
	ContBB< OBB > obbs;
	ContNode< ComponentCollide > children;

	const bool level_collide( const ComponentCollide& other ) const
	{
		for ( const auto& aabb : aabbs )
		{
			for ( const auto& oaabb : other.aabbs )
			{
				if ( aabb.collide( oaabb ) ) return true;
			}

			for ( const auto& oobb : other.obbs )
			{
				if ( aabb.collide( make_AABB_from_OBB( oobb ) ) ) return true;
			}
		}

		for ( const auto& obb : obbs )
		{
			for ( const auto& oobb : other.obbs )
			{
				if ( obb.collide( oobb ) ) return true;
			}

			for ( const auto& oaabb : other.aabbs )
			{
				if ( obb.collide( make_OBB_from_AABB( oaabb ) ) ) return true;
			}
		}

		return false;
	}

	static OBB make_OBB_from_AABB( const AABB& aabb )
	{
		OBB ret;
		ret.init( aabb.coord->get_base(), aabb.half_len, aabb.movement );
		return ret;
	}

	static AABB make_AABB_from_OBB( const OBB& obb )
	{
		AABB ret;
		ret.init( obb.coord->get_base(), obb.half_len, obb.movement );
		return ret;
	}
};

bool ComponentCollide::will_get_desc = true;

#endif