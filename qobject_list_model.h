#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>
#include <QVariant>

namespace __qobjectsqmllist {
	// try to test header function declaration exists
	template<class> struct type_sink { typedef void type; }; // consumes a type, and makes it `void`
	template<class T> using type_sink_t = typename type_sink<T>::type;
	template<class T, class=void> struct has_getHeader : std::false_type {}; \
	template<class T> struct has_getHeader<
		T,
		type_sink_t< decltype( std::declval<T>().getHeader() ) >
	>: std::true_type {};

	namespace details {
		template<class T>
		QVariant optionalGetHeader_helper(T* obj, std::true_type /*has_getHeader*/) {
			return obj->getHeader();
		}
		template<class T>
		QVariant optionalGetHeader_helper(T* /*obj*/, std::false_type /*has_getHeader*/) {
			return QVariant();
		}
	}
	template<class T>
	QVariant optionalGetHeader(T* obj) {
		return details::optionalGetHeader_helper( obj, has_getHeader<T>{} );
	}

	// role name to be used in delegates for list items
	static const char ITEM_ROLE_NAME[] = "modelData";

	// instantiating optimization.
	// since we don't need actual model index when we work with
	// flat list, we can instantiate it just once and use everywhere
	static const QModelIndex ROOT_MODEL_INDEX;

	template <typename T>
	class QObjectListModelBase : public QAbstractListModel, public QList<QSharedPointer<T>> {
		using ITEM = QSharedPointer<T>;
		using LIST = QList<ITEM>;

	public:
		enum CollectionRole {
			ItemDataRole = Qt::UserRole
		};

		int rowCount( const QModelIndex& parent ) const override {
			Q_UNUSED( parent )
			return LIST::count();
		}

		QVariant data( const QModelIndex& index, int role ) const override {
			auto row = index.row();

			if ( row < 0 || row >= LIST::count() )
				return QVariant();

			if ( role == ItemDataRole )
				return QVariant::fromValue<QObject*>( LIST::at( row ).data() );

			return QVariant();
		}

		QHash<int, QByteArray> roleNames() const override {
			auto roles = QAbstractListModel::roleNames();
			roles[ItemDataRole] = __qobjectsqmllist::ITEM_ROLE_NAME;
			return roles;
		}

		QVariant headerData(int section, Qt::Orientation orientation, int /*role*/ = Qt::DisplayRole) const override {
			QVariant res;
			if ((LIST::count() > 0) && (orientation == Qt::Horizontal)) {
				QVariant header = optionalGetHeader(LIST::at(0).data());
				switch (header.type()) {
					case QVariant::List:
						res = header.toList().at(section);
						break;
					case QVariant::String:
						res = header.toString();
					default:
						break;
				}
			}
			return res;
		}

		void insert( int i, const ITEM& item ) {
			if ( i < 0 || i > LIST::count() )
				return;

			beginInsertRows( __qobjectsqmllist::ROOT_MODEL_INDEX, i, i );
			LIST::insert( i, item );
			endInsertRows();
			emitChanged();
		}

		void append( const ITEM& item ) {
			auto i = LIST::count();
			beginInsertRows( __qobjectsqmllist::ROOT_MODEL_INDEX, i, i );
			LIST::append( item );
			endInsertRows();
			emitChanged();
		}

		void append( const LIST& list ) {
			if ( list.count() == 0 )
				return;

			auto start = LIST::count();
			auto end = LIST::count() + list.count() - 1;
			beginInsertRows( __qobjectsqmllist::ROOT_MODEL_INDEX, start, end );
			LIST::append( list );
			endInsertRows();
			emitChanged();
		}

		ITEM getAt( int i ) {
			if ( i < 0 || i > LIST::count() - 1 )
				return nullptr;

			return LIST::value( i );
		}

		ITEM getFromPointer( T * ptr ) {
			if (! ptr)
				return nullptr;

			for (int i = 0 ; i < LIST::count() ; ++i) {
				ITEM item = LIST::value(i);
				if (item.data() == ptr) {
					return item;
				}
			}
			return nullptr;
		}

		bool contains(const T * ptr) {
			if (! ptr)
				return false;

			for (int i = 0 ; i < LIST::count() ; ++i) {
				if (LIST::value(i).data() == ptr) {
					return true;
				}
			}
			return false;
		}

		void removeAt( int i ) {
			if ( i < 0 || i > LIST::count() - 1 )
				return;

			beginRemoveRows( __qobjectsqmllist::ROOT_MODEL_INDEX, i, i );
			LIST::removeAt( i );
			endRemoveRows();
			emitChanged();
		}

		bool removeOne( const ITEM& item ) {
			int index = LIST::indexOf( item );
			if ( index != -1 ) {
				removeAt( index );
				return true;
			}
			return false;
		}

		void removeLast() {
			if ( LIST::count() == 0 )
				return;

			auto i = LIST::count() - 1;
			beginRemoveRows( __qobjectsqmllist::ROOT_MODEL_INDEX, i, i );
			LIST::removeAt( i );
			endRemoveRows();
			emitChanged();
		}

		void removeFirst() {
			if ( LIST::count() == 0 )
				return;

			beginRemoveRows( __qobjectsqmllist::ROOT_MODEL_INDEX, 0, 0 );
			LIST::removeAt( 0 );
			endRemoveRows();
			emitChanged();
		}

		void replace( int i, const ITEM& item ) {
			removeAt( i );
			insert( i, item );
			emitChanged();
		}

		void clear() {
			beginResetModel();
			LIST::clear();
			endResetModel();
			emitChanged();
		}

		// --- QList-style comfort ;) ---

		QObjectListModelBase& operator+=( const ITEM& t ) {
			append( t );
			return *this;
		}

		QObjectListModelBase& operator<<( const ITEM& t ) {
			append( t );
			return *this;
		}

		QObjectListModelBase& operator+=( const LIST& list ) {
			append( list );
			return *this;
		}

		QObjectListModelBase& operator<<( const LIST& list ) {
			append( list );
			return *this;
		}

	protected:
		virtual void emitChanged() = 0;
	};
}

/*
 * QML ListView generic c++ model for collection of QObject-s.
 * Usage:
 * 1) Declare new List Model:
 *      DECLARE_Q_OBJECT_LIST_MODEL( app::DataItem )
 * 2) Register types in Qt Meta-system:
 *      qmlRegisterUncreatableType<app::DataItem>( "App", 1, 0, "DataItem", "interface" );
 *      qmlRegisterUncreatableType<app::QObjectList_DataItem>( "App", 1, 0, "DataItemList", "interface" );
 * 3) Define and implement Q_PROPERTY:
 *      Q_PROPERTY( QObjectList<app::DataItem>* items READ items CONSTANT )
 * 4) Use this property as simple QList<QSharedObject<app::DataItem>>
 * 5) Use "modelData" conventional injected property to get access to list item data in ListView Delegates.
 *
 * In QML ListView delegate the ROLE "modelData" will be available to get access to list item.
 * Since this role is conventional when using ListView with JS arrays, it is very handy
 * to use JS arrays as mock data providers during view prototyping.
 *
 * NOTE: Not all QList methods are usable to get this thing work. Please refer to this class source
 * and feel free to implement the rest methods.
 * Also, since QList doesn't have that methods marked as "virtual"
 * it is impossible to use this class with pointer to QList<>*.
 *
 * @author Vadim Usoltsev
 */
#define DECLARE_LIB_Q_OBJECT_LIST_MODEL( LIB_IMPORT_EXPORT_DECLARATION, TYPE )                                                                                  \
	class LIB_IMPORT_EXPORT_DECLARATION OLM_##TYPE : public __qobjectsqmllist::QObjectListModelBase<TYPE> {                                                \
		Q_OBJECT                                                                                                             \
		Q_PROPERTY( int length READ count NOTIFY changed )                                                                   \
		Q_PROPERTY( int isEmpty READ isEmpty NOTIFY changed )                                                                \
																															 \
	Q_SIGNALS:                                                                                                               \
		void changed();                                                                                                      \
																															 \
	protected:                                                                                                               \
		virtual void emitChanged() { emit changed(); }                                                                       \
																															 \
		Q_INVOKABLE QVariant item( int i ) {                                                                                 \
			if ( i < 0 || i >= count() )                                                                                     \
				return QVariant();                                                                                           \
																															 \
			auto obj = at( i ).data();                                                                                       \
			QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );                                                 \
			return QVariant::fromValue( obj );                                                                               \
		}                                                                                                                    \
	public:                                                                                                                  \
		TYPE * objPtr(int i) { return getAt( i ).data(); }                                                                   \
		Q_INVOKABLE QVariant header(int section) const {                                                                     \
			return headerData(section, Qt::Horizontal, Qt::DisplayRole);                                                     \
		}                                                                                                                    \
	};
#define NO_LIB
#define DECLARE_Q_OBJECT_LIST_MODEL( TYPE ) DECLARE_LIB_Q_OBJECT_LIST_MODEL(NO_LIB, TYPE)
