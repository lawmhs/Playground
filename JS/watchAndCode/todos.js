// Code goes here

var todoList = {
  todos: [],
  displayTodos: function() {

    if (this.todos.length === 0) {
        console.log("Your todo list is empty!");
    } else {
        console.log('My Todos:');
        for(var i = 0; i < this.todos.length; i++) {

            if(this.todos[i].completed === true) {
              console.log('(x)', this.todos[i].todoText);
            } else {
              console.log('( )', this.todos[i].todoText)
            }
        }
    }
  },
  addTodo: function(todoText) {
    this.todos.push({
      todoText: todoText,
      completed: false
    });
    this.displayTodos();
  },
  changeTodo: function(position, todoText) {
    this.todos[position].todoText = todoText;
    this.displayTodos();
  },
  deleteTodo: function(position) {
    this.todos.splice(position, 1);
    this.displayTodos();
  },
  toggleCompleted: function(position) {
    var todo = this.todos[position]; // this works
    // because its not a copy of an object
    // its a reference
    // to whatever is stored at this.todos[position], that is, the address of the object represented by this.todos[position]

    // however, if you grabbed todo as this.todos[position].completed
    // then its a primitive and it'll be a copy of it
    // since when javascript makes a new variable
    // and you assign an old variable to it
    // then it'll grab whatever is stored in that variable
    // so changing that one won't change the other
    todo.completed = !todo.completed;
    this.displayTodos();
  },
  toggleAll: function() {
    var totalTodos = this.todos.length;
    var completedTodos = 0;

    // Get number of completed todos.
    for(var i = 0; i < totalTodos; i++) {
      if(this.todos[i].completed === true) {
        completedTodos++;
      }
    }

    // Case 1 : If everything's true, make everything false
    if(completedTodos === totalTodos) {
      for(var i = 0; i < totalTodos; i++) {
        this.todos[i].completed = false;
      }
    } else {
      for(var i = 0; i < totalTodos; i++) {
        this.todos[i].completed = true;
      }
    }
    this.displayTodos();
  }
};

// button handling
var displayTodosButton = document.getElementById('displayTodosButton');
var toggleAllButton = document.getElementById('toggleAllButton');

displayTodosButton.addEventListener('click', function() {
  todoList.displayTodos();
});

toggleAllButton.addEventListener('click', function() {
  todoList.toggleAll();
});