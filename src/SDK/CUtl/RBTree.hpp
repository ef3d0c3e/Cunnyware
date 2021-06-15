#ifndef SDK_CUTL_RBTREE_HPP
#define SDK_CUTL_RBTREE_HPP

#include "Memory.hpp"

template <class I>
struct UtlRBTreeLinks
{
	I left;
	I right;
	I parent;
	I tag;
};

template <class T, class I>
struct UtlRBTreeNode : public UtlRBTreeLinks<I>
{
	T data;
};

template <class T, class I = unsigned short, typename L = bool (*)(const T&, const T&), class M = CUtlMemory<UtlRBTreeNode<T, I>, I>>
class CUtlRBTree : non_constructible
{
public:
	typedef T KeyType_t;
	typedef T ElemType_t;
	typedef I IndexType_t;

	// Less func typedef
	// Returns true if the first parameter is "less" than the second
	typedef L LessFunc;
protected:
	MAKE_CENUMV_Q(NodeColor, i32,
		RED, 0,
		BLACK, 1);

	typedef UtlRBTreeNode<T, I> Node;
	typedef UtlRBTreeLinks<I> Links;

	M elements;
	I root;
	I numElements;
	I firstFree;
	typename M::Iterator lastAlloc; // the last index allocated

	Node* pElements;

	inline M const& Elements() const
	{
		return elements;
	}
};


#endif // SDK_CUTL_RBTREE_HPP
