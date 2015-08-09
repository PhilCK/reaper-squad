
# Technical Design Doc

This isn't going into a lot of detail as for the most part this game is a hobby, and I do things time depending. This doc is both for the Caffeine Library and the Application it self.

## Application

The goal of the application is to generate a flying/space game.

### Main Design Philosophy

- MVC Design Pattern
- Event Driven

### MVC

The view should have no access to the controller, and the controller should only access the model. Communication with other entities should be restricted as much as possible to events.

### External Libraries

- Caffeine (through project directory not linking)

## Caffeine

The goal of this project is to create a library that can be used for other projects. Caffeine will have two levels. Application which will be standalone so it can be used for small projects, and the rest which will for the engine.

### Main Design Philosophy

- Data Driven Content
- Hot-loading
- Event Driven

### Application

Application is a standalone section and should have no dependencies with other elements of Caffeine (Other than utilities).

### Services

Services are the main point of contact for the application.

### Systems

Systems make up the backbone of the engine.

### External Libraries

- SDL / OpenGL 3.2
- tinyXML
- GLM
- SOIL
- Bullet Physics
- Rift SDK

## Technical Goals

# Primary Goals

- Basic Caffeine Functionality
- Forward Lighting
- Basic AI
- No Animation
- Basic GUI
- Basic Skybox

# Secondary Goals

- Oculus Rift
- Good AI
- Comic Strip GUI

# Tertiary Goals

- Deferred lighting/rendering
- Post Process effects
- Good Skybox
