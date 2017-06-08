#include <Rcpp.h>
#include "RMySQL_types.h"
using namespace Rcpp;

// [[Rcpp::export]]
XPtr<MyConnectionPtr> connection_create(
    const Rcpp::Nullable<std::string>& host,
    const Rcpp::Nullable<std::string>& user,
    const Rcpp::Nullable<std::string>& password,
    const Rcpp::Nullable<std::string>& db,
    unsigned int port,
    const Rcpp::Nullable<std::string>& unix_socket,
    unsigned long client_flag,
    const Rcpp::Nullable<std::string>& groups,
    const Rcpp::Nullable<std::string>& default_file,
    const Rcpp::Nullable<std::string>& ssl_key,
    const Rcpp::Nullable<std::string>& ssl_cert,
    const Rcpp::Nullable<std::string>& ssl_ca,
    const Rcpp::Nullable<std::string>& ssl_capath,
    const Rcpp::Nullable<std::string>& ssl_cipher,
    unsigned int timeout) {
  MyConnectionPtr* pConn = new MyConnectionPtr(
    new MyConnection(host, user, password, db, port, unix_socket, client_flag,
      groups, default_file, ssl_key, ssl_cert, ssl_ca, ssl_capath, ssl_cipher, timeout)
  );
  return XPtr<MyConnectionPtr>(pConn, true);
}

// [[Rcpp::export]]
void connection_release(XPtr<MyConnectionPtr> con) {
  if ((*con)->hasQuery()) {
    warning("%s\n%s",
      "There is a result object still in use.",
      "The connection will be automatically released when it is closed"
    );
  }
  return con.release();
}

// [[Rcpp::export]]
List connection_info(XPtr<MyConnectionPtr> con) {
  return (*con)->connectionInfo();
}

// [[Rcpp::export]]
bool connection_valid(XPtr<MyConnectionPtr> con) {
  return con.get() != NULL;
}

// [[Rcpp::export]]
CharacterVector connection_quote_string(XPtr<MyConnectionPtr> con,
                                         CharacterVector input) {
  int n = input.size();
  CharacterVector output(n);

  for (int i = 0; i < n; ++i) {
    if (input[i] == NA_STRING) {
      output[i] = NA_STRING;
    } else {
      String x = input[i];
      output[i] = "'" + (*con)->quoteString(x) + "'";
    }
  }

  return output;
}

// [[Rcpp::export]]
bool connection_exec(XPtr<MyConnectionPtr> con, std::string sql) {
  return (*con)->exec(sql);
}
