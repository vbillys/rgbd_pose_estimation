/**
 * \file CentralAbsoluteAdapter.hpp
 * \brief Adapter-class for passing bearing-vector-to-point correspondences to
 *        the central absolute-pose algorithms. It maps opengv types
 *        back to opengv types.
 */

#ifndef _AO_POSE_ADAPTER_HEADER_
#define _AO_POSE_ADAPTER_HEADER_

#include <stdlib.h>
#include <vector>
#include <PnPPoseAdapter.hpp>

using namespace std;
using namespace Eigen;
/**
 * \brief The namespace for the absolute pose methods.
 */

/**
 * Check the documentation of the parent-class to understand the meaning of
 * an AbsoluteAdapter. This child-class is for the central case and holds data
 * in form of references to opengv-types.
 */
template<typename POSE_T, typename POINT_T>
class AOPoseAdapter : public PnPPoseAdapter<POSE_T, POINT_T>
{

protected:
	using PoseAdapterBase<POSE_T, POINT_T>::_t_w;
	using PoseAdapterBase<POSE_T, POINT_T>::_R_cw;
	using PnPPoseAdapter<POSE_T, POINT_T>::_bearingVectors;
	using PnPPoseAdapter<POSE_T, POINT_T>::_points_g;

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	typedef typename PoseAdapterBase<POSE_T, POINT_T>::Vector3 Vector3;
	typedef typename PoseAdapterBase<POSE_T, POINT_T>::SO3_T SO3_T;
	typedef typename PoseAdapterBase<POSE_T, POINT_T>::Point3 Point3;
	
	typedef typename PnPPoseAdapter<POSE_T, POINT_T>::MatrixX MatrixX;

  /**
   * \brief Constructor. See protected class-members to understand parameters
   */
   AOPoseAdapter(
      const MatrixX & bearingVectors,
	  const MatrixX & points_c,
	  const MatrixX & points_g);
  /**
   * \brief Constructor. See protected class-members to understand parameters
   */
  AOPoseAdapter(
	  const MatrixX & bearingVectors,
	  const MatrixX & points_c,
	  const MatrixX & points_g,
      const SO3_T & R );
  /**
   * \brief Constructor. See protected class-members to understand parameters
   */
  AOPoseAdapter(
	  const MatrixX & bearingVectors,
	  const MatrixX & points_c,
	  const MatrixX & points_g,
      const Vector3 & t,
      const SO3_T & R );
  /**
   * Destructor
   */
  virtual ~AOPoseAdapter();

  //Access of correspondences
  
  bool isInlier33(int index) const;
  POINT_T weight33(int index) const;
  /** See parent-class */
  virtual Point3 getPointCurr( int index ) const;
  virtual bool isValid( int index ) const;
  virtual void setInlier(const Matrix<short, Dynamic, Dynamic>& inliers);
  virtual void setWeights(const Matrix<short, Dynamic, Dynamic>& weights);
  virtual void printInlier() const;
  const vector<short>& getInlierIdx() const { return _vInliersAO; }
  void cvtInlier();

protected:
  /** Reference to the 3-D points in the camera-frame */
  const MatrixX & _points_c; 
  /** flags of inliers. */
  Matrix<short, Dynamic, 1> _inliers_3d;
  Matrix<short, Dynamic, 1> _weights_3d;
  vector<short> _vInliersAO;
};


template<typename POSE_T, typename POINT_T>
AOPoseAdapter<POSE_T, POINT_T>::AOPoseAdapter(
	const MatrixX & bearingVectors,
	const MatrixX & points_c,
	const MatrixX & points_g) :
	PnPPoseAdapter<POSE_T, POINT_T>(bearingVectors, points_g),
	_points_c(points_c)
{
	_inliers_3d.resize(_bearingVectors.cols());
	_inliers_3d.setOnes();
}

template<typename POSE_T, typename POINT_T>
AOPoseAdapter<POSE_T, POINT_T>::AOPoseAdapter(
	const MatrixX & bearingVectors,
	const MatrixX & points_c,
	const MatrixX & points_g,
	const SO3_T & R) :
	PnPPoseAdapter<POSE_T, POINT_T>(bearingVectors, points_g, R),
	_points_c(points_c)
{
	_inliers_3d.resize(_bearingVectors.cols());
	_inliers_3d.setOnes();
}

template<typename POSE_T, typename POINT_T>
AOPoseAdapter<POSE_T, POINT_T>::AOPoseAdapter(
	const MatrixX & bearingVectors,
	const MatrixX & points_c,
	const MatrixX & points_g,
	const Vector3 & t,
	const SO3_T & R) :
	PnPPoseAdapter<POSE_T, POINT_T>(bearingVectors, points_g, t, R),
	_points_c(points_c)
{
	_inliers_3d.resize(_bearingVectors.cols());
	_inliers_3d.setOnes();
}

template<typename POSE_T, typename POINT_T>
AOPoseAdapter<POSE_T, POINT_T>::~AOPoseAdapter()
{}

template<typename POSE_T, typename POINT_T>
typename AOPoseAdapter<POSE_T, POINT_T>::Point3 AOPoseAdapter<POSE_T, POINT_T>::getPointCurr(
	int index) const
{
	assert(index < _bearingVectors.cols() );
	return _points_c.col(index);
}

template<typename POSE_T, typename POINT_T>
bool AOPoseAdapter<POSE_T, POINT_T>::isValid( int index ) const
{
	assert(index < _bearingVectors.cols());
	return _points_c.col(index)(0) == _points_c.col(index)(0) || _points_c.col(index)(1) == _points_c.col(index)(1) || _points_c.col(index)(2) == _points_c.col(index)(2);
}

template<typename POSE_T, typename POINT_T>
bool AOPoseAdapter<POSE_T, POINT_T>::isInlier33(int index) const
{
	assert(index < _inliers_3d.rows());
	return _inliers_3d(index) == 1;
}

template<typename POSE_T, typename POINT_T>
POINT_T AOPoseAdapter<POSE_T, POINT_T>::weight33(int index) const
{
	if (!_weights_3d.rows()) return POINT_T(1.0);
	else{
		assert(index < _weights_3d.rows());
		return POINT_T(_weights_3d(index)) / numeric_limits<short>::max();
	}
}

template <typename POSE_T, typename POINT_T>
void AOPoseAdapter<POSE_T, POINT_T>::setInlier(const Matrix<short, Dynamic, Dynamic>& inliers)
{
	assert(inliers.rows() == _inliers_3d.rows());
	if (inliers.cols() == 1){
		PnPPoseAdapter<POSE_T, POINT_T>::setInlier(inliers);
	}
	else{
		PnPPoseAdapter<POSE_T, POINT_T>::setInlier(inliers);
		_inliers_3d = inliers.col(1);
		//memcpy(_inliers_3d.data(), inliers.col(1).data(), inliers.rows() * 2);
	}
	return;
}

template <typename POSE_T, typename POINT_T>
void AOPoseAdapter<POSE_T, POINT_T>::setWeights(const Matrix<short, Dynamic, Dynamic>& weights)
{
	if (weights.rows() == 1){
		PnPPoseAdapter<POSE_T, POINT_T>::setWeights(weights);
	}
	else{
		PnPPoseAdapter<POSE_T, POINT_T>::setWeights(weights);
		_weights_3d = weights.col(1);
		//memcpy(_weights_3d.data(), weights.col(1).data(), weights.rows() * 2);
	}
	return;
}

template <typename POSE_T, typename POINT_T>
void AOPoseAdapter<POSE_T, POINT_T>::printInlier() const
{
	PnPPoseAdapter<POSE_T, POINT_T>::printInlier();
	cout << _inliers_3d.transpose() << endl;
}

template <typename POSE_T, typename POINT_T>
void AOPoseAdapter<POSE_T, POINT_T>::cvtInlier()
{
	_vInliersAO.clear();
	_vInliersAO.reserve(getNumberCorrespondences());
	for (short r = 0; r < (short)_inliers_3d.rows(); r++) {
		if (1 == _inliers_3d[r]){
			_vInliersAO.push_back(r);
		}
	}
}

#endif 
