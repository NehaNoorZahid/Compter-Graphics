// void pressKeySpecial(int key, int x, int y)
// {
//     switch (key)
//     {
//     case GLUT_KEY_LEFT:
//         m.setDirection(Mario::LEFT);
//         break;
    
//     case GLUT_KEY_RIGHT:
//         m.setDirection(Mario::RIGHT);
//         break;
    
//     case GLUT_KEY_UP:
//         m.setDirection(Mario::UP);
//         break;
    
//     case GLUT_KEY_DOWN:
//         m.setDirection(Mario::DOWN);
//         break;
//     }
//     glutPostRedisplay();
// }

// void releaseKeySpecial(int key, int x, int y)
// {
//     switch (key)
//     {
//     case GLUT_KEY_LEFT:
//     case GLUT_KEY_RIGHT:
//     case GLUT_KEY_UP:
//     case GLUT_KEY_DOWN:
//         m.setDirection(Mario::NONE); // Stop movement in all directions
//         break;
//     }
//     glutPostRedisplay();
// }