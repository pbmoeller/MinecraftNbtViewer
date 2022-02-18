#include "nbt_data_treenode.hpp"

namespace anv
{

NbtDataTreeNode::NbtDataTreeNode(const QVector<QVariant> &data, 
                                 NbtDataTreeNode *parentItem)
    : m_itemData(data)
    , m_parent(parentItem)
{

}

NbtDataTreeNode::~NbtDataTreeNode()
{

}

int NbtDataTreeNode::columnCount() const
{
    return m_itemData.count();
}

} // namespace anv