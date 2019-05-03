// Code goes here

var todoList = {
  todos: [],
  addTodo: function(todoText) {
    this.todos.push({
      todoText: todoText,
      completed: false
    });
  },
  changeTodo: function(position, todoText) {
    this.todos[position].todoText = todoText;
  },
  deleteTodo: function(position) {
    this.todos.splice(position, 1);
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
  },
  toggleAll: function() {
    var totalTodos = this.todos.length;
    var completedTodos = 0;

    // Get number of completed todos.
    this.todos.forEach(function(todo) {
      if(todo.completed === true) {
        completedTodos++;
      }
    });
    
    this.todos.forEach(function(todo) {
      if(completedTodos === totalTodos) {
        // case 1 : if everything's true, make everything false
        todo.completed = false;
      } else {
        // otherwise, make everything true
        todo.completed = true;
      }
    });
  }
};

var handlers = {
  addTodo: function() {
    var addTodoTextInput = document.getElementById('addTodoTextInput');
    todoList.addTodo(addTodoTextInput.value);
    addTodoTextInput.value = ''; // remember that addTodoTextInput is a reference to the object
    // that is referenced by documents.getElementById
    // since everything is a reference in JS
    addTodoTextInput.className = 'todoItem';
    view.displayTodos();
  },
  changeTodo: function(id) {
    var changeTodoPositionInput = document.getElementById(id);

    todoList.changeTodo(id, changeTodoPositionInput.value);
    changeTodoPositionInput.value = '';
    view.displayTodos();
  },
  deleteTodo: function(position) {
    todoList.deleteTodo(position);
    view.displayTodos();
  },
  toggleCompleted: function(position) {
    todoList.toggleCompleted(position); // this is a number I guess it seems
    view.displayTodos();
  },
  toggleAll: function() {
    todoList.toggleAll();
    view.displayTodos();
  }
};

var view = {
  displayTodos: function() {
    
    // need to clear the current ul first
    // otherwise you'll add duplicate items
    var todosUl = document.querySelector('ul');
    todosUl.innerHTML = ''; // inner HTML grabs whatever is inside the <ul> tag
  
    // a callback is out of scope
    // only know what is passed into the forEach
    // so you can pass in this like so:
    // forEach(callback, this)
    // and then the callback will know what "this" refers to
    // this is optional
    
    todoList.todos.forEach(function(todo, position) {
      var todoButton = this.createDoneButton();
      var todoLi = document.createElement('li');
      todoLi.className = 'todoText';
      
      if(todo.completed === true) {
        todoButton.textContent = '(x)';
      } else {
        todoButton.textContent = '( )';
      }
      
      todoLi.id = position;
      todoLi.textContent = todo.todoText;
      todoLi.appendChild(todoButton); // append a done-ness button
      todoLi.appendChild(this.createDeleteButton()); // gives the li element a button child in the DO
      todosUl.appendChild(todoLi);
    }, this);
  },
  createDeleteButton: function() {
    
    // make a button and return it
    var deleteButton = document.createElement('button');
    deleteButton.textContent = 'Delete';
    deleteButton.className = 'deleteButton';
    return deleteButton;
    // returns a button of the form <button class="deleteButton">Delete</buton>
  },
  createDoneButton: function() {
    // make a button to toggle doneness and return it
    var doneButton = document.createElement('button');
    doneButton.textContent = '( )';
    doneButton.className = 'doneButton';
    return doneButton;
  },
  setUpEventListeners: function() {
    // this sets up the event listener for the delete button
    var todosUl = document.querySelector('ul');

    // only need to have an event listener for the ul since it encompasses all of the li elements
    todosUl.addEventListener('click', function(event) {
      // Get the element that was clicked on.
      var elementClicked = event.target;
      
      // Check if elementClicked is a delete button (use the class)
      if(elementClicked.className === 'deleteButton'){
        // Run handlers.deleteTodo
        handlers.deleteTodo(parseInt(elementClicked.parentNode.id));
      } else if(elementClicked.className === 'doneButton') {
        //debugger;
        handlers.toggleCompleted(elementClicked.parentNode.id);
      } else if(elementClicked.className === 'todoText') {
        
        // we need an event listener in here to know to replace
        // the input element once we press enter
        // or click out of it
        var changeTodoInput = document.createElement("input");
        changeTodoInput.setAttribute('type', 'text');
        changeTodoInput.setAttribute('id', elementClicked.id);

        // replace the list element with our new input element
        var targetParent = elementClicked.parentNode;
        targetParent.replaceChild(changeTodoInput, elementClicked);
      
        // automatically set user input to the text box created
        changeTodoInput.focus();
        changeTodoInput.select();
        
        // now we listen 
        // or an event here and run some stuff it so happens we're changing an element
        changeTodoInput.addEventListener('keypress', function(event) {
          if(event.key === 'Enter') {
            handlers.changeTodo(changeTodoInput.id);
          }
        });
        
      }
    });
    
    
    // now we need event listeners for onkeypress
    var addTodosInput = document.getElementById('addTodoTextInput');
    
    addTodosInput.addEventListener('keypress', function(event){
      if(event.key === 'Enter') {
        // if its enter, run addTodo
        handlers.addTodo();
      }
    });
    
  }
    
};

view.setUpEventListeners();