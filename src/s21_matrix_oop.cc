#include "s21_matrix_oop.h"

using namespace std;

S21Matrix::S21Matrix() { Nullify(); }

S21Matrix::S21Matrix(int rows, int cols) { CreateMatrix(rows, cols); }

S21Matrix::S21Matrix(const S21Matrix& other) { CopyMatrix(other); }

S21Matrix::S21Matrix(S21Matrix&& other) {
  CopyMatrix(other);
  other.FreeMatrix();
  other.Nullify();
}

S21Matrix::~S21Matrix() { FreeMatrix(); }

void S21Matrix::CreateMatrix(int rows, int cols) {
  if (rows < 1 || cols < 1) {
    throw length_error("Error! Either rows or columns are less than 1");
  }
  cols_ = cols;
  rows_ = rows;
  matrix_ = new double*[rows_];
  if (matrix_ == nullptr) {
    Nullify();
    throw bad_alloc();
  }
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_];
    if (matrix_[i] == nullptr) {
      for (int j = 0; j <= i; j++) {
        delete[] matrix_[j];
      }
      delete[] this->matrix_;
      Nullify();
      throw bad_alloc();
    }
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = 0.0;
    }
  }
}

void S21Matrix::CopyMatrix(const S21Matrix& other) {
  CreateMatrix(other.GetRows(), other.GetCols());
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  } else {
    throw bad_alloc();
  }
}

void S21Matrix::FreeMatrix() {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
    Nullify();
  }
}

void S21Matrix::Nullify() {
  rows_ = 0;
  cols_ = 0;
  matrix_ = nullptr;
}

int S21Matrix::GetCols() const { return cols_; }

void S21Matrix::SetCols(int cols) {
  S21Matrix newmatrix;
  if (cols < 1) {
    throw length_error("Incorrect input for setCols");
  }
  newmatrix = S21Matrix(rows_, cols);
  if (newmatrix.matrix_ == nullptr) {
    throw bad_alloc();
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols; j++) {
      newmatrix(i, j) = j < cols_ ? matrix_[i][j] : 0;
    }
  }
  FreeMatrix();
  CopyMatrix(newmatrix);
  cols_ = cols;
  newmatrix.FreeMatrix();
}

int S21Matrix::GetRows() const { return rows_; }

void S21Matrix::SetRows(int rows) {
  S21Matrix newmatrix;
  if (rows < 1) {
    throw length_error("Incorrect input for setRows");
  }
  newmatrix = S21Matrix(rows, cols_);
  if (newmatrix.matrix_ == nullptr) {
    throw bad_alloc();
  }
  for (int j = 0; j < cols_; j++) {
    for (int i = 0; i < rows; i++) {
      newmatrix(i, j) = i < rows_ ? matrix_[i][j] : 0;
    }
  }
  FreeMatrix();
  CopyMatrix(newmatrix);
  newmatrix.FreeMatrix();
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (matrix_ == nullptr || other.matrix_ == nullptr || rows_ < 1 ||
      cols_ < 1 || other.GetCols() < 1 || other.GetRows() < 1 ||
      matrix_ == nullptr || other.matrix_ == nullptr ||
      rows_ != other.GetRows() || cols_ != other.GetCols()) {
    throw length_error("Incorrect input for SumMatrix");
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] = matrix_[i][j] + other.matrix_[i][j];
      }
    }
  }
}

bool S21Matrix::EqMatrix(const S21Matrix& other) {
  bool res = true;
  if (matrix_ == nullptr || other.matrix_ == nullptr || rows_ < 1 ||
      cols_ < 1 || other.GetCols() < 1 || other.GetRows() < 1 ||
      rows_ != other.GetRows() || cols_ != other.GetCols()) {
    throw length_error("Incorrect input for EqMatrix");
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        if (fabs(matrix_[i][j] - other.matrix_[i][j]) > 1e-7) {
          res = false;
          break;
        }
      }
      if (!res) {
        break;
      }
    }
  }
  return res;
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (matrix_ == nullptr || other.matrix_ == nullptr || rows_ < 1 ||
      cols_ < 1 || other.GetCols() < 1 || other.GetRows() < 1 ||
      rows_ != other.GetRows() || cols_ != other.GetCols()) {
    throw length_error("Incorrect input for SumMatrix");
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] = matrix_[i][j] - other.matrix_[i][j];
      }
    }
  }
}

void S21Matrix::MulNumber(const double num) {
  if (matrix_ == nullptr) {
    throw length_error("Incorrect input for MulNumber");
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] *= num;
      }
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  S21Matrix tmp;
  if (matrix_ == nullptr || other.matrix_ == nullptr ||
      cols_ != other.GetRows()) {
    throw length_error("Incorrect input for MulMatrix");
  } else {
    tmp = S21Matrix(rows_, other.GetCols());
    if (tmp.matrix_ == nullptr) {
      throw bad_alloc();
    } else {
      MulMatrixCycle(other, tmp);
      FreeMatrix();
      CopyMatrix(tmp);
      tmp.FreeMatrix();
    }
  }
}

void S21Matrix::MulMatrixCycle(const S21Matrix& other, S21Matrix& tmp) const {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.GetCols(); j++) {
      tmp.matrix_[i][j] = 0;
      for (int k = 0; k < other.GetRows(); k++) {
        tmp.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
}

S21Matrix S21Matrix::Transpose() {
  S21Matrix res;
  if (matrix_ == nullptr) {
    throw length_error("Incorrect input for Transpose");
  } else {
    res = S21Matrix(cols_, rows_);
    if (res.matrix_ != nullptr) {
      for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
          res.matrix_[j][i] = matrix_[i][j];
        }
      }
    } else {
      throw bad_alloc();
    }
  }
  return res;
}

S21Matrix S21Matrix::CalcComplements() {
  S21Matrix result;
  S21Matrix minor;
  double temp = 0;
  if (matrix_ == nullptr || rows_ != cols_) {
    throw length_error("Incorrect input for CalcComplements");
  } else {
    result = S21Matrix(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        minor = Minority(i, j);
        temp = minor.Determinant();
        result.matrix_[i][j] = pow(-1, j + i + 2) * temp;
        minor.FreeMatrix();
      }
    }
  }
  return result;
}

S21Matrix S21Matrix::Minority(int row, int col) const {
  if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
    throw std::length_error("Invalid input for Minority()");
  }
  S21Matrix minor(rows_ - 1, cols_ - 1);
  int minor_row = 0;
  for (int i = 0; i < rows_; i++) {
    if (i != row) {
      int minor_col = 0;
      for (int j = 0; j < cols_; j++) {
        if (j != col) {
          minor(minor_row, minor_col) = matrix_[i][j];
          minor_col++;
        }
      }
      minor_row++;
    }
  }
  return minor;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) {
  S21Matrix tmp(*this);
  tmp.SumMatrix(other);
  return tmp;
}

S21Matrix S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) {
  S21Matrix tmp(*this);
  tmp.SubMatrix(other);
  return tmp;
}

S21Matrix S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) {
  S21Matrix tmp(*this);
  tmp.MulMatrix(other);
  return tmp;
}

S21Matrix S21Matrix::operator*(const double num) {
  S21Matrix tmp(*this);
  tmp.MulNumber(num);
  return tmp;
}

S21Matrix S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator*=(const double num) {
  MulNumber(num);
  return *this;
}

bool S21Matrix::operator==(const S21Matrix& other) { return EqMatrix(other); }

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  if (this != &other) {
    FreeMatrix();
    CopyMatrix(other);
  }
  return *this;
}

double& S21Matrix::operator()(int i, int j) {
  if (matrix_ == nullptr || i >= rows_ || j >= cols_ || i < 0 || j < 0) {
    throw length_error("Incorrect input for operator()");
  }
  return matrix_[i][j];
}

double& S21Matrix::operator()(int i, int j) const {
  if (matrix_ == nullptr || i >= rows_ || j >= cols_ || i < 0 || j < 0) {
    throw length_error("Incorrect input for operator()");
  }
  return matrix_[i][j];
}

double S21Matrix::Determinant() {
  double result = 0;
  if (matrix_ == nullptr || rows_ != cols_) {
    throw length_error("Incorrect input for Determinant");
  } else {
    if (rows_ == 1) {
      result = matrix_[0][0];
    } else if (rows_ == 2) {
      result = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
    } else {
      for (int j = 0; j < cols_; j++) {
        S21Matrix minor = Minority(0, j);
        double temp = minor.Determinant();
        result += pow(-1.0, j) * temp * matrix_[0][j];
        minor.FreeMatrix();
      }
    }
  }
  return result;
}

S21Matrix S21Matrix::InverseMatrix() {
  double determinant = 0;
  S21Matrix calc_complements;
  S21Matrix transpont;
  S21Matrix result;
  if (matrix_ == nullptr || rows_ != cols_) {
    throw length_error("Incorrect input for InverseMatrix");
  } else {
    determinant = Determinant();
    if (determinant == 0) {
      throw length_error("Inverse matrix doesn't exist");
    }
    calc_complements = CalcComplements();
    transpont = calc_complements.Transpose();
    transpont.MulNumber(1 / determinant);
    result = S21Matrix(transpont);
    calc_complements.FreeMatrix();
    transpont.FreeMatrix();
  }
  return result;
}
