#ifndef AWESOMENBTVIEWER_MODELS_NBT_DATA_TREENODE_HPP
#define AWESOMENBTVIEWER_MODELS_NBT_DATA_TREENODE_HPP

// Qt
#include <QVector>

namespace anv
{

class NbtDataTreeNode
{
public:
    NbtDataTreeNode(const QVector<QVariant> &data, 
                    NbtDataTreeNode *parentItem = nullptr);
    ~NbtDataTreeNode();

    int columnCount() const;

private:
    QVector<QVariant> m_itemData;
    NbtDataTreeNode *m_parent;
    QVector<NbtDataTreeNode*> m_children;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_MODELS_NBT_DATA_TREENODE_HPP