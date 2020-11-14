#include "knn.h"
#include <Eigen/Dense>
#include <iostream>

int Predict1toMaxNeighbors(
    double *train_inputs_ptr, 
    double *train_label_ptr, 
    int nrow, int ncol, int max_neighbors, 
    double *test_input_ptr, //ncol
    //inputs above, outputs below
    double *test_prediction_ptr //max neighbors
){
  Eigen::VectorXd distance_vec(nrow);
  Eigen::Map< Eigen::MatrixXd > train_inputs_mat(train_inputs_ptr, nrow, ncol);
  Eigen::Map< Eigen::VectorXd > test_input_vec(test_input_ptr, ncol);
  Eigen::VectorXd diff_vec(ncol);
  Eigen::VectorXi sorted_index_vec(nrow);
  //Compute the distance
  for(int i=0;i<nrow;i++){
    distance_vec(i) = (train_inputs_mat.row(i).transpose()-test_input_vec).lpNorm<1>();
    //distance_vec(i) = diff_vec.lpNorm<1>();//computes L1 norm
    //distance_vec(i) = diff_vec.norm(); //Computes L2 norm
    sorted_index_vec(i) = i; //initialze sort vec
  }
  
  std::sort(sorted_index_vec.data(),
            sorted_index_vec.data()+sorted_index_vec.size(),
            [&distance_vec](int left, int right){             //&distance_vec lets you use distance_vec in this func
              return distance_vec(left) < distance_vec(right);
            });
  //std:: cout << sorted_index_vec << std::endl << std:: endl;
  double total_labels = 0.0;
  // First element to max neighbors
  for(int k=0; k<max_neighbors; k++){
    int row = sorted_index_vec(k);
    int neighbors = k+1;
    total_labels+= train_label_ptr[row];
    test_prediction_ptr[k]= total_labels/neighbors;
  }
  return 0;
}

int knn(
    const double *train_inputs_ptr, //n_observations x n_features (Matrix)
    const double *train_label_ptr,  //n_observations (vector)
    const double *test_input_ptr,   //n_features (vector)
    const int n_observations, const int n_features, const int max_neighbors,
    double *test_predictions_ptr    //max_neighbors (vector)
){
  Eigen::Map< Eigen::MatrixXd > train_inputs_mat((double*)train_inputs_ptr, n_observations,n_features);
  Eigen::Map< Eigen::VectorXd > test_input_vec((double*)test_input_ptr,n_features);
  Eigen::VectorXd dist_vec(n_observations);
  Eigen::VectorXi sorted_index_vec(n_observations);
  for (int i=0; i<n_observations;i++){
    dist_vec(i) = (train_inputs_mat.row(i).transpose() - test_input_vec).norm();
    sorted_index_vec(i) = i;
  }
  std::sort(
    sorted_index_vec.data(),
    sorted_index_vec.data()+sorted_index_vec.size(),
    [&dist_vec](int left, int right){
      return dist_vec(left) < dist_vec(right);
    }
  );
  double total = 0.0;
  for(int model_i=0; model_i<max_neighbors; model_i++){
    int neighbors = model_i +1;
    int row_i = sorted_index_vec(model_i);
    total+=train_label_ptr[row_i];
    test_predictions_ptr[model_i] = total/neighbors;
  }
  return 0;
}