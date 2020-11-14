int knn(
    const double *train_inputs_ptr, //n_observations x n_features (Matrix)
    const double *train_label_ptr,  //n_observations (vector)
    const double *test_input_ptr,   //n_features (vector)
    const int n_observations, const int n_features, const int max_neighbors,
    double *test_predictions_ptr    //max_neighbors (vector)
);
int Predict1toMaxNeighbors(
    double *train_inputs_ptr, 
    double *train_label_ptr, 
    int nrow, int ncol, 
    int max_neighbors, 
    double *test_input_ptr, 
    double *test_prediction_ptr
);