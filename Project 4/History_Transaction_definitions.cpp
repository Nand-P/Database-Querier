#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

//g++ -o main History_Transaction_definitions.cpp project4.cpp main.cpp -std=c++11

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
// 
//

// Constructor
// TASK 1
//

Transaction::Transaction( std::string ticker_symbol, unsigned int day_date, unsigned int month_date,
unsigned year_date, bool buy_sell_trans, unsigned int number_shares, double trans_amount
) {

    symbol = ticker_symbol;
    day = day_date;
    month = month_date;
    year = year_date;
    if ( buy_sell_trans == true ) {

      trans_type = "Buy";

    } else if ( buy_sell_trans == false ) {

      trans_type = "Sell";

    }
    
    shares = number_shares;
    amount = trans_amount;
    
    trans_id = assigned_trans_id++;

    acb = 0;
    acb_per_share = 0;
    share_balance = 0;
    cgl = 0;

    p_next = nullptr;

}

// Destructor
// TASK 1
//

Transaction::~Transaction() {

}

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<( Transaction const &other ) {

  if (year == other.year) {
    
    if (month == other.month) {

      if (day == other.day) {

        if (trans_id > other.trans_id) {

          return true;

        }

      } else if (day < other.day) {

        return true;

      }
    
    } else if (month < other.month) {

      return true;

    }

  } else if (year < other.year) {

    return true;

  }

  return false;

}



// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//
History::History() {
  
  //An empty linked list is only one node, which means it does not point to anything. This means it is set to nullptr. 
  p_head = nullptr;

}

// Destructor
// TASK 3
//
History::~History() {
  
  //current_node will lag 1 node behind to delete objects (starts at first node)
  Transaction *temp = nullptr;
  Transaction *current_node = p_head;
  
  //Run until it reaches the final node.
  while (current_node != nullptr) {
    
    //Moves temp to the next node ahead of the one being deleted.
    temp = current_node -> get_next();

    //Frees the memory at current_node.
    delete current_node;

    //Moves current_node to temp.
    current_node = temp;
  
  }

  delete temp;
  temp = nullptr;
  current_node = nullptr;
 
  /* for loop attempt
  //Begins at the second node of the linked list and while it is has not reached the end, move to the next node.
  for (Transaction *current_node = p_head -> get_next(); current_node != nullptr; current_node = previous_node -> get_next()) {

    delete previous_node;

    previous_node = current_node;
  
  }

  //Deletes the last node after current_node fails the for loop condition.
  delete previous_node;
  previous_node = nullptr;
  */

  
}


// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history() {
  
  ece150::open_file();
  
  while (ece150::next_trans_entry()) {

    Transaction *newNode {new Transaction { 

        ece150::get_trans_symbol(),
        ece150::get_trans_day(),
        ece150::get_trans_month(),
        ece150::get_trans_year(),
        ece150::get_trans_type(),
        ece150::get_trans_shares(),
        ece150::get_trans_amount()

      }

    };

    insert(newNode);

  }

  ece150::close_file();

  return;
  
}


// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction *p_new_trans) {

  //Pointer to hold the address of the last node in the linked list.
  Transaction *last_node = p_head;
  
  if (p_head == nullptr) {

    p_head = p_new_trans;

  } else {

    while (last_node -> get_next() != nullptr) {
      
      last_node = last_node -> get_next();

    }

    last_node -> set_next(p_new_trans);

  }

  /* For loop attempt
  //Starts at current node and runs through the linked list until it reaches the last node
  for (Transaction *current_node = p_head; current_node != nullptr; current_node = current_node -> get_next()) {
    
    last_node = current_node;

  }
  */

  return;

}


// sort_by_date(): Sort the linked list by trade date.
// TASK 6
// 
//WARNING: My sort_by_date function looks like a minefield. It took me several hours of cumulative effort to figure it out, but it is done.
void History::sort_by_date() {
  
  //This is a variable for the "original" linked list.
  Transaction *removed_node = p_head;

  //These are variables for the sorted linked list.
  Transaction *previous_node = p_head;
  Transaction *temp_head = p_head;
  Transaction *cycle_new_list = p_head;
  
  
  //ATTEMPT 1
  /*BLOCK 1
  //current_node saves the spot in the original linked list, saves the address of the next node.
  removed_node = current_node;
  current_node = current_node -> get_next();

  //Moves p_head to the next node
  p_head = current_node;
    
  //Removes node from linked list without deleting it.
  removed_node -> set_next(nullptr);
  temp = removed_node;
  */

  //This code only runs to remove the first node from the original linked list (no comparisons are used, so it is special case)
  p_head = p_head -> get_next();
  temp_head -> set_next(nullptr); 

  //While we haven't reached the last node in the original linked list.
  while (p_head -> get_next() != nullptr) {

    //Resets back to the first node in the sorted linked list.
    cycle_new_list = temp_head;

    //This removes the node from the original linked list and updates p_head
    removed_node = p_head;
    p_head = p_head -> get_next();
    removed_node -> set_next(nullptr);

    //EXAMINE THIS WHILE CONDITION (CONTINUE WORKING HERE) CONSIDER ADDING CODE OUTSIDE OF BOTH WHILE LOOPS TO ACCOUNT FOR WHEN WE REMOVE THE FIRST NODE.
    while (cycle_new_list != nullptr) {

      if (*removed_node < *cycle_new_list) {
        
        if (cycle_new_list == temp_head) {
          
          //Adds removed_node to the front of the linked list.
          removed_node -> set_next(temp_head);
          temp_head = removed_node;
          break;
        
        } //else if (cycle_new_list -> get_next() == nullptr) {
          
          //Adds the removed_node to the end of the linked list.
          // cycle_new_list -> set_next(removed_node);
          // break;

        //} 
        else {
          
          //Inserts node into a spot (not first or last) in the linked list.
          removed_node -> set_next(cycle_new_list);
          previous_node -> set_next(removed_node);
          break;

        }

      } else if (cycle_new_list -> get_next() != nullptr){
        
        previous_node = cycle_new_list;
        cycle_new_list = cycle_new_list -> get_next();

      } else {

        cycle_new_list -> set_next(removed_node);
        break;

      }

    }

  }

  cycle_new_list = temp_head;

  while (cycle_new_list != nullptr) {

    if (*p_head < *cycle_new_list) {
        
      if (cycle_new_list == temp_head) {
          
        //Adds removed_node to the front of the linked list.
        p_head -> set_next(temp_head);
        temp_head = p_head;
        break;
        
      } else {
          
        //Inserts node into a spot (not first or last) in the linked list.
        previous_node -> set_next(p_head);
        p_head -> set_next(cycle_new_list);
        break;

      }

    } else if (cycle_new_list -> get_next() != nullptr) {
        
      previous_node = cycle_new_list;
      cycle_new_list = cycle_new_list -> get_next();

    } else {

      cycle_new_list -> set_next(p_head);
      break;

    }

  }

  p_head = temp_head;

  removed_node = nullptr;
  previous_node = nullptr;
  temp_head = nullptr;
  cycle_new_list = nullptr;
  
  /* ATTEMPT 1
  while (current_node != nullptr) {

    Transaction *previous_node = p_head;
    Transaction *current_node = p_head;
    Transaction *removed_node = p_head;
    Transaction *temp = p_head;

    bool isTrue = true;
    //MAYBE UNFINISHED HERE

    //BLOCK 1
    //current_node cycles through the linked list, saves the address of the next node.
    while (isTrue) {

      current_node = current_node -> get_next();

      //Moves p_head to the second node
      p_head = current_node;
      temp = current_node;
    
      //Removes node from linked list without deleting it.
      removed_node -> set_next(nullptr);

      isTrue = false;

    }

    //BLOCK 2
    while (temp != nullptr) {

      if (*removed_node < *temp) {
        
        //BLOCK 3
        if (temp == p_head) {
          
          //Adds removed_node to the front of the linked list.
          removed_node -> set_next(current_node);
          p_head = removed_node;
        
        } else if (temp -> get_next() == nullptr) {
          
          //Adds the removed_node to the end of the linked list.
          temp -> set_next(removed_node);

        } else {
          
          //Inserts node into a spot (not first or last) in the linked list
          removed_node -> set_next(current_node);
          previous_node -> set_next(removed_node);

        }

      } else {
        
        previous_node = temp;
        temp = temp -> get_next();

      }

    }

    //UNFINISHED HERE

  }
  */
 
  return;

}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//

void History::update_acb_cgl() {

  Transaction *previous_node = p_head;
  Transaction *current_node = p_head -> get_next();
  double cgl{0};

  //Sets ACB for head transaction
  p_head -> set_acb(p_head -> get_amount());

  //Sets number of shares for head transaction
  p_head -> set_share_balance(p_head -> get_shares());
  
  //Sets ACB per share for head transaction.
  p_head -> set_acb_per_share(p_head -> get_acb() / p_head -> get_share_balance());

  while (current_node != nullptr) {

    //For type Buy transactions
    if (current_node -> get_trans_type()) {

      //Computes ACB
      current_node -> set_acb(previous_node -> get_acb() + current_node -> get_amount()); 

      //Computes new share balance
      current_node -> set_share_balance(previous_node -> get_share_balance() + current_node -> get_shares());

      //Computes ACB per share
      current_node -> set_acb_per_share(current_node -> get_acb() / current_node -> get_share_balance());  
      
    } else { //For type Sell transactions

      //Computes ACB
      current_node -> set_acb(previous_node -> get_acb() - (current_node -> get_shares() * previous_node -> get_acb_per_share()));       

      //Computes new share balance
      current_node -> set_share_balance(previous_node -> get_share_balance() - current_node -> get_shares());

      //Since no new ACB/Share needs to be calculated, the previous transaction's ACB/Share is copied
      current_node -> set_acb_per_share(previous_node -> get_acb_per_share());

      cgl = current_node -> get_amount() - (current_node -> get_shares() * previous_node -> get_acb_per_share());

      current_node -> set_cgl(cgl);
      
      cgl = 0;

    }

    //Moves to the next node in the linked list
    previous_node = current_node;
    current_node = current_node -> get_next();

  }
  
  return;

}

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8

double History::compute_cgl(unsigned int year) {

  Transaction *previous_node = p_head;
  Transaction *current_node = p_head -> get_next();
  double cgl{0};
  double total_cgl{0};

  while (current_node != nullptr) {

    if ((current_node -> get_trans_type() == false) && (current_node -> get_year() == year)) {

      cgl = current_node -> get_amount() - (current_node -> get_shares() * previous_node -> get_acb_per_share());

      total_cgl += cgl;

    }

    previous_node = current_node;
    current_node = current_node -> get_next();

  }


  return total_cgl;

}


// print() Print the transaction history.
//TASK 9
//

void History::print() {

  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;

  Transaction *current_node = p_head;

  while (current_node != nullptr) {

    current_node -> print();

    current_node = current_node -> get_next();

  }

  /* For loop attempt
  for (Transaction *current_node = p_head; current_node != nullptr; current_node = current_node -> get_next()) {
    
    if (current_node != nullptr) {

      current_node -> print();

    }
    
  }
  */

  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;

  return;

}

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }