/*!
 * \file  utils.h
 *
 * \brief Declares class Utils.
 */

#ifndef UTILS_H
#define UTILS_H

#include <QList>

class QTreeView;
class QVariant;
class QModelIndex;
class QAbstractItemModel;

namespace QxRunner {

/*!
 * \brief The Utils class provides a set of static helper functions.
 *       
 *
 * The methods of this class provide logic that must otherwise be
 * coded identically at different locations in the project. It's not
 * possible to create an instance of the class.
 */

class Utils
{
public: // Operations

	/*!
	 * Returns the width of the columns in the \a view.
	 */
	static QList<int> columnSizes(QTreeView* view);

	/*!
	 * Returns the icon that represents \a result which must be a
	 * QxRunner::RunnerResult value.
	 */
	static QVariant resultIcon(int result);

	/*!
	 * Returns the source model of the proxy \a model.
	 */
	static QAbstractItemModel* modelFromProxy(QAbstractItemModel* model);

	/*!
	 * Returns the source model index corresponding to the given proxy
	 * \a index from the proxy \a model.
	 */
	static QModelIndex modelIndexFromProxy(QAbstractItemModel* model,
                                           const QModelIndex& index);

	/*!
	 * Returns the model index in the proxy \a model given the source
	 * \a index from the source model.
	 */
	static QModelIndex proxyIndexFromModel(QAbstractItemModel* model,
                                           const QModelIndex& index);

private: // Operations

	/*!
	 * Virtual destructor required when there are virtual methods.
	 */
	virtual ~Utils() {};

	/*!
	 * Prevents from creating an instance of this class.
	 */
	virtual void dummy() = 0;
};

} // namespace

#endif // UTILS_H
