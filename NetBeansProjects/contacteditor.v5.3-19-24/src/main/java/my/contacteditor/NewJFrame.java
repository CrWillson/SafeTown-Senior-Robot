/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/GUIForms/JFrame.java to edit this template
 */
package my.contacteditor;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.geom.*;
import java.io.IOException;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.*;
import javax.swing.*;
import javax.swing.AbstractAction;
import javax.swing.border.AbstractBorder;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.KeyStroke;
import javax.swing.JToggleButton;





/**
 *
 * @author tojo9
 */
    
public class NewJFrame extends javax.swing.JFrame {
    
    //this variable saves which button is currently selected 
    private int saveButton;
    // Declare and initialize an ArrayList to hold JToggleButtons
    private ArrayList<OverlayButton> toggleButtonList = new ArrayList<>();
    //private ArrayList<JToggleButton> toggleButtonList = new ArrayList<>(); 
    
    
    private int colSize = 1;
    private int rowSize = 1;
    private int initialSize = 100;
    private int initialOverlaySize = 33;
    private int scaleBy = 10;
    private int OriginalImages = 6;
    private int TotalNumImages = OriginalImages * 4 + 1;
     
    
    ImageIcon[] scaledIcons = new ImageIcon[TotalNumImages];
    //initializes the rotation for each road square
        //each rotation is 90 degrees apart
    private int rotate[] = new int[1]; 
    private boolean setOverlayTrue[] = new boolean[1];
    
    //initializes the road type for each road square
        //type 5 = blank
        //type 0 = straight road
        //type 1 = curved road
        //type 2 = 3-way-intersection
        //type 3 = 4-way-intersection
        //type 4 = 3-way-intersection-mirrored 
        //type 5 = empty
    private int road[] = new int[1];
    JPanel panel = new JPanel();
  
    /**
     * Creates new form NewJFrame
     */
    public NewJFrame() {
        initComponents();
        setupKeyboardShortcuts();
        jScrollPane1.setViewportView(panel);
    }
    
//****************************Layout Setup*********************************
//**************************************************************************    
    
     private void JBInputMapSizeAction() {
        resizeRotateRoad(colSize*rowSize);
        scaleBy = -5;
        if(jScrollPane1.getWidth()/colSize > jScrollPane1.getHeight()/rowSize){
            initialSize = jScrollPane1.getWidth()/colSize;
        }
        else{
            initialSize = jScrollPane1.getHeight()/rowSize;
        }
        System.out.println(jScrollPane1.getHeight());
        System.out.println(jScrollPane1.getWidth());
        System.out.println(initialSize);
        
        panel.setLayout(new GridLayout(rowSize, colSize));

        // Clear the existing grid of buttons
        panel.removeAll();
        toggleButtonList.clear();
        for(int i = 0; i < rowSize*colSize; i++){
            createButtons();
        }
        
        setIcon();  
        
    }
    
    private void createButtons(){
        // Add the panel to the JScrollPane  
        //JToggleButton button = new JToggleButton();
        OverlayButton button = new OverlayButton(null, null);
        button.setSize(new Dimension(initialSize, initialSize));
        button.setBorder(null);
        final int buttonIndex = toggleButtonList.size();  
        button.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/Button-Background.png")));
        road[buttonIndex] = 5;
        setOverlayTrue[buttonIndex] = false;
        
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                saveButton = buttonIndex;
                imageLayer();
                ButtonToggle();
        }});
        panel.add(button); // Add the button to jPanel1
        toggleButtonList.add(button); // Add the button to the ArrayList
     }

    //sets new size of grid for road, rotate, and setOverlayTrue
    private void resizeRotateRoad(int newSize) {
        int[] newRotate = new int[newSize];
        System.arraycopy(rotate, 0, newRotate, 0, Math.min(rotate.length, newRotate.length));
        rotate = newRotate;
        
        int[] newRoad = new int[newSize];
        System.arraycopy(road, 0, newRoad, 0, Math.min(road.length, newRoad.length));
        road = newRoad;
        
        boolean[] newSetOverlayTrue = new boolean[newSize];
        System.arraycopy(setOverlayTrue, 0, newSetOverlayTrue, 0, Math.min(setOverlayTrue.length, newSetOverlayTrue.length));
        setOverlayTrue = newSetOverlayTrue;
    } 
//****************************Setup Icons***********************************
//**************************************************************************
     private void setIcon(){
                   
            updateIconSize();

            // Force layout update to reflect changes
            panel.revalidate();
            panel.repaint();
        
     }
     
     //sets the selected road tile to a specific icon
    private void IconInsert(){
        //selects the tile that will have it's icon changed
        //uses the road type and rotation to call the scaledIconSelect function which selects the icon
        //once the icon is selected the icon is set to the road tile selected
       toggleButtonList.get(saveButton).setIcon(scaledIcons[road[saveButton]*4 + rotate[saveButton]]);      
    }
    
    //finds the icon for each specific case
    private String IconSelect(int RoadType, int RotateNum){
        //sets icon to blank
        String icon = "";
        
        //finds which road type is being used
            //sets the varible icon to the image file path
        switch (RoadType){
            case 0:
                switch (RotateNum){
                    case 0:
                        icon = "/images/straight-road.png";
                        break;
                    case 1:
                        icon = "/images/straight-road-R1.png";
                        break;
                    case 2:
                        icon = "/images/straight-road-R2.png";
                        break;
                    case 3:
                        icon = "/images/straight-road-R3.png";
                        break;
                }
                break;
            case 1:
                switch (RotateNum){
                    case 0:
                        icon = "/images/curve-road.png";
                        break;
                    case 1:
                        icon = "/images/curve-road-R1.png";
                        break;
                    case 2:
                        icon = "/images/curve-road-R2.png";
                        break;
                    case 3:
                        icon = "/images/curve-road-R3.png";
                        break;
                }
                break;
            case 2:
                switch (RotateNum){
                    case 0:
                        icon = "/images/3-way-intersection.png";
                        break;
                    case 1:
                        icon = "/images/3-way-intersection-R1.png";
                        break;
                    case 2:
                        icon = "/images/3-way-intersection-R2.png";
                        break;
                    case 3:
                        icon = "/images/3-way-intersection-R3.png";
                        break;
                }
                break;
            case 3:
                switch (RotateNum){
                    case 0:
                        icon = "/images/4-way-intersection.png";
                        break;
                    case 1:
                        icon = "/images/4-way-intersection-R1.png";
                        break;
                    case 2:
                        icon = "/images/4-way-intersection-R2.png";
                        break;
                    case 3:
                        icon = "/images/4-way-intersection-R3.png";
                        break;
                }
                break; 
            case 4:
                switch (RotateNum){
                    case 0:
                        icon = "/images/3-way-intersection.mirror.png";
                        break;
                    case 1:
                        icon = "/images/3-way-intersection.mirror-R1.png";
                        break;
                    case 2:
                        icon = "/images/3-way-intersection.mirror-R2.png";
                        break;
                    case 3:
                        icon = "/images/3-way-intersection.mirror-R3.png";
                        break;
                }
                break;
            case 5:
                icon = "/images/Button-Background.png";
                break;
            case 6:
                icon = "/images/store-image.jpg";
                break;
        }
        
        //returns the file path to the image
        return(icon);
    }
     
     private void updateIconSize(){
        try {   
            
            for(int i = 0; i < OriginalImages ; i++){
                for(int j = 0; j < 4; j++){   
                    // Load image from file
                    Image img = ImageIO.read(NewJFrame.class.getResource(IconSelect(i, j)));

                    // Create ImageIcon from loaded image
                    Image scaledImage = img.getScaledInstance(initialSize + scaleBy, initialSize + scaleBy, Image.SCALE_SMOOTH);
                    ImageIcon scaledIcon = new ImageIcon(scaledImage);
                    scaledIcons[i*4 + j] = scaledIcon;
                }
            }
            Image img = ImageIO.read(NewJFrame.class.getResource(IconSelect(6, 0)));
            Image scaledImage = img.getScaledInstance((initialSize + scaleBy - 15)/3, (initialSize + scaleBy - 15)/3, Image.SCALE_SMOOTH);
            ImageIcon scaledIcon = new ImageIcon(scaledImage);
            scaledIcons[TotalNumImages - 1] = scaledIcon;
            
                // Update icons for components
            for(int i = 0; i < colSize*rowSize; i++){
                if(toggleButtonList.get(i).getIcon() != null){
                    toggleButtonList.get(i).setIcon(scaledIcons[road[i]*4 + rotate[i]]);
                    if(setOverlayTrue[i] == true){
                        toggleButtonList.get(i).setOverlayIcon(scaledIcons[TotalNumImages - 1]);
                    }
                }
            }
            
        } catch (IOException e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(this, "Error loading image: " + e.getMessage(), "Image Loading Error", JOptionPane.ERROR_MESSAGE);
        }
        
     }

//****************************Additonal Functions***************************
//**************************************************************************     
     
    private void CurveRoadSelect(){
        //sets road type to curved road
        road[saveButton] = 1;                                       
        rotate[saveButton] = 0;
        
        //calls the function that inserts the needed icon
        IconInsert(); 
    }    
    private void ThreeWayIntersectionSelect(){
        //sets road type to 3-way 
        road[saveButton] = 2;                                       
        rotate[saveButton] = 0;
        
        //calls the function that inserts the needed icon
        IconInsert();     
    }
    private void FourWayIntersectionSelect(){
            //sets road type to 4-way
        road[saveButton] = 3;                                       
        rotate[saveButton] = 0;
        
        //calls the function that inserts the needed icon
        IconInsert();
    }
    private void StraightRoadSelect(){
            //sets road type to straight road
        road[saveButton] = 0;                    
        rotate[saveButton] = 0;
        
        //calls the function that inserts the needed icon
        IconInsert(); 
    }
    
    private void scaleUp(){
         scaleBy = scaleBy + 10;
     }
    
     private void scaleDown(){
         scaleBy = scaleBy - 10;
     
     }
   
    
    private void ButtonRotate(){
            if(rotate[saveButton] < 3){
                rotate[saveButton] = rotate[saveButton] + 1;                                                    
            }
            else{
                rotate[saveButton] = 0;
            }
        IconInsert();
    }
    private void ButtonMirror(){
        //toggles between a mirrored 3-way and the original 3-way
        if (road[saveButton] == 4){
            //sets to normal 3-way
            road[saveButton] = 2;  
            
            //mirrors along the axis from mirrored to normal
            if (rotate[saveButton] < 1){
            rotate[saveButton] = rotate[saveButton] + 3;
            }
            else{
                rotate[saveButton] = rotate[saveButton] - 1;
            }
        }
        else{
            //sets to mirrored 3-way
            road[saveButton] = 4;
            
            //mirrors along the axis from normal to mirrored 
            if (rotate[saveButton] < 3){
                rotate[saveButton] = rotate[saveButton] + 1;
            }
            else{
                rotate[saveButton] = rotate[saveButton] - 3;
            }
        }
        
        //calls the function that inserts the needed icon
        IconInsert();    
    }
    private void ButtonDelete(int ButtonNum){
        //sets road type to Blank
        road[ButtonNum] = 5;
        
        //sets blank icon
        IconInsert();
    }
    
     private void ButtonInputMapSize(){
        try {
            colSize = Integer.parseInt(jTextField1.getText()); // Try to parse the text as an integer
            rowSize = Integer.parseInt(jTextField2.getText());
        } catch (NumberFormatException ex) {
            // If parsing fails, show a warning message

        }
        JBInputMapSizeAction();
    }
    
    //toggles road tiles so only one tile can be pressed at once
    private void ButtonToggle(){
        //sets all tiles selected to false
        for(int i = 0; i <colSize*rowSize; i++){
        toggleButtonList.get(i).setSelected(false);
        toggleButtonList.get(i).setBorder(null);
        }
        //finds the tile currently selected and turns the selection on
        toggleButtonList.get(saveButton).setSelected(true);
        toggleButtonList.get(saveButton).setBorder(new IconBorder(Color.RED));
        jPanel1.requestFocusInWindow();     
    }

//****************************Icon Border and Shortcuts*********************
//**************************************************************************
    
    //creates a border around the selected image
    class IconBorder extends AbstractBorder {
        private Color borderColor;

        public IconBorder(Color borderColor) {
            this.borderColor = borderColor;
        }

        @Override
        public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
            super.paintBorder(c, g, x, y, width, height);

            // Draw the border around the icon
            Graphics2D g2d = (Graphics2D) g;
            g2d.setColor(borderColor);
            g2d.setStroke(new BasicStroke(2)); // Adjust border thickness as needed
            g2d.drawRect(x, y, width - 1, height - 1); // Adjust border position and size as needed
        }

    }
    
    // Set up the keyboard shortcut
    private void setupKeyboardShortcuts() {    
        setKeyboardShortcut(KeyEvent.VK_A, 0, "A");
        setKeyboardShortcut(KeyEvent.VK_D, 0, "D");
        setKeyboardShortcut(KeyEvent.VK_E, 0, "E");
        setKeyboardShortcut(KeyEvent.VK_R, 0, "R");
        setKeyboardShortcut(KeyEvent.VK_M, 0, "M");    
        setKeyboardShortcut(KeyEvent.VK_1, 0, "1");
        setKeyboardShortcut(KeyEvent.VK_2, 0, "2");
        setKeyboardShortcut(KeyEvent.VK_3, 0, "3");
        setKeyboardShortcut(KeyEvent.VK_4, 0, "4");
        setKeyboardShortcut(KeyEvent.VK_UP, 0, "UP");
        setKeyboardShortcut(KeyEvent.VK_RIGHT, 0, "RIGHT");
        setKeyboardShortcut(KeyEvent.VK_DOWN, 0, "DOWN");
        setKeyboardShortcut(KeyEvent.VK_LEFT, 0, "LEFT");
        setKeyboardShortcut(KeyEvent.VK_MINUS, 0, "MINUS");
        setKeyboardShortcut(KeyEvent.VK_EQUALS, 0, "EQUALS");
}
    
    //watches for keyboard inputs and triggers the action associated with it
    private void setKeyboardShortcut(int keyEvent, int modifiers, String actionKey) {
        KeyStroke keyStroke = KeyStroke.getKeyStroke(keyEvent, modifiers);
        getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(keyStroke, actionKey);
        getRootPane().getActionMap().put(actionKey, new AbstractAction() {
            @Override
        public void actionPerformed(ActionEvent e) {
            // The logic for when a specific keyboard shortcut is triggered
            if ((jTextField1.isFocusOwner() == false) && (jTextField2.isFocusOwner() == false)) {                
                switch (actionKey) {
                // The logic for when a specific keyboard shortcut is triggered
                    case "1":
                        StraightRoadSelect();
                        break;
                    case "2":
                        CurveRoadSelect();
                        break;
                    case "3":
                        ThreeWayIntersectionSelect();
                        break;
                    case "4":
                        FourWayIntersectionSelect();
                        break;
                    case "A":
                        setOverlayTrue[saveButton] = true;
                        imageLayer();
                        break;
                    case "R":
                        ButtonRotate();
                        break;            
                    case "D":
                        ButtonDelete(saveButton);
                        break; 
                    case "E":
                        deleteLayer();
                        break;
                    case "M":
                        ButtonMirror();;
                        break;    
                     case "UP":
                            if(saveButton >= colSize){ 
                                saveButton = saveButton - colSize;
                            }
                            else{
                                saveButton = colSize*rowSize + saveButton - colSize;
                            }
                            ButtonToggle();
                        break;
                    case "RIGHT":
                            if(saveButton <= rowSize*colSize - 2){ 
                                saveButton = saveButton + 1;
                            }
                            else{
                                saveButton = 0;
                            }
                            ButtonToggle();
                        break;
                    case "DOWN":
                            if(saveButton <= rowSize*colSize - colSize -1){ 
                                saveButton = saveButton + colSize;
                            }
                            else{
                                saveButton = saveButton - rowSize*colSize + colSize;
                            } 
                            ButtonToggle();
                        break;
                    case "LEFT":
                            if(saveButton >= 1){ 
                                saveButton = saveButton - 1;
                            }
                            else{
                                saveButton = rowSize*colSize - 1;
                            }
                            ButtonToggle();
                        break; 
                    case "MINUS":
                        scaleDown();
                        setIcon();
                        break;
                    case "EQUALS":
                        scaleUp();
                        setIcon();
                        break;                    
                }
                imageLayer(); // Call your method to handle the overlay image
            } 

            
        }
            
            
        });
        
    }
    
//****************************DESIGN RULE CHECK*****************************
//**************************************************************************    
  
    
    private void userFeedBack() { 
        ResultText.setText("Please Display");
    }
    
    private void designRuleCheck(){
        String returnText;
        boolean isValid = true;
        int[] checkValid = new int[8];
        int[] checkNext = new int[8];
        ResultText.setText("");
        for (int i = 0; i < toggleButtonList.size(); i++){
            checkValid = checkIfValid(road[i],rotate[i]);
            isValid = true;
            //checks if road above is valid
            if(i >= colSize){ //checks if the tiles is at a edge    
                checkNext = checkIfValid(road[i - colSize], rotate[i - colSize]);  
                if(checkValid[0] == 1){
                    if(checkNext[5] == 0){
                        isValid = false;
                        System.out.println("above left is invalid");
                    }
                }
                else if(checkValid[1] == 1){
                    if(checkNext[4] == 0){
                        isValid = false;
                        System.out.println("above right is invalid");
                    }
                }
            }
            else if(checkValid[0] == 1 || checkValid[1] == 1){
                isValid = false;
                System.out.println("Above is edge");
            }  
            //checks if the road to the right is valid
            if((i+1) % colSize != 0){ //checks if the tiles is at a edge    
                checkNext = checkIfValid(road[i + 1], rotate[i + 1]);  
                if(checkValid[2] == 1){
                    if(checkNext[7] == 0){
                        isValid = false;
                        System.out.println("right up is invalid");
                    }
                }
                else if(checkValid[3] == 1){
                    if(checkNext[6] == 0){
                        isValid = false;
                        System.out.println("right down is invalid");
                    }
                }
            }
            else if(checkValid[2] == 1 || checkValid[3] == 1){
                isValid = false;
                System.out.println("right is edge");
            } 
            //checks if the road below is valid
            if(i <= rowSize*colSize - colSize -1){ //checks if the tiles is at a edge    
                checkNext = checkIfValid(road[i + colSize], rotate[i + colSize]);  
                if(checkValid[4] == 1){
                    if(checkNext[1] == 0){
                        isValid = false;
                        System.out.println("below right is invalid");
                    }
                }
                else if(checkValid[5] == 1){
                    if(checkNext[0] == 0){
                        isValid = false;
                        System.out.println("below left is invalid");
                    }
                }
            }
            else if(checkValid[4] == 1 || checkValid[5] == 1){
                isValid = false;
                System.out.println("down is edge");
            } 
            //checks if the road to the left is valid
            if(i % colSize != 0){ //checks if the tiles is at a edge    
                checkNext = checkIfValid(road[i - 1], rotate[i - 1]);  
                if(checkValid[6] == 1){
                    if(checkNext[3] == 0){
                        isValid = false;
                        System.out.println("left down is invalid");
                    }
                }
                else if(checkValid[7] == 1){
                    if(checkNext[2] == 0){
                        isValid = false;
                        System.out.println("left up is invalid");
                    }
                }
            }
            else if(checkValid[6] == 1 || checkValid[7] == 1){
                isValid = false;
                System.out.println("Left is edge");
            }  
            
            
            if (isValid == false){
                int roadnum = i+1;
                returnText = "Road " + roadnum + " Is not in a Valid location";
                ResultText.append("\n" + returnText);
            }
            else{
               int roadnum = i+1;
               returnText = "Valid" + roadnum;
               ResultText.append("\n" + returnText); 
            }
            System.out.println("Road is " + i);
        }
    }
    
    
    private int[] checkIfValid(int roadType, int rotateType){
        int[] findEdges = new int[8];
        switch(roadType){
            case 0: //straight road
                        findEdges = new int[]{1, 0, 0, 0, 0, 1, 0, 0};
                break;
            case 1: //curve road
                        findEdges = new int[]{0, 0, 1, 0, 0, 1, 0, 0};
                break;
            case 2: // 3-way
                        findEdges = new int[]{1, 0, 1, 0, 0, 1, 0, 0};
                break;
            case 3: // 4-way
                        findEdges = new int[]{1, 0, 1, 0, 0, 1, 0, 1};
                break;
            case 4: // 3-way mirror
                        findEdges = new int[]{0, 0, 1, 0, 0, 1, 0, 1};                
                break;
            case 5: // empty
                        findEdges = new int[]{0, 0, 0, 0, 0, 0, 0, 0};
                break;
        }
        int[] reference = new int[8];
        for(int j = 0; j < rotateType; j++){   
            System.arraycopy(findEdges, 0, reference, 0, 8);
            for(int i = 0; i < 8; i++){
                if(i < 2){
                findEdges[i] = reference[6 + i];   
                }
                else{
                    findEdges[i] = reference[i-2];
                }

            }
        }
        for(int i = 0; i < 8; i++){
            System.out.println(findEdges[i]);
        }
            System.out.println("next");
        
        
        
        return findEdges;
    }
    
    
//****************************image overlap*********************************
//**************************************************************************  
    private void imageLayer(){
        if(setOverlayTrue[saveButton] == true){
            toggleButtonList.get(saveButton).setOverlayIcon(scaledIcons[TotalNumImages - 1]);
        }
        int y = toggleButtonList.get(saveButton).getHeight();
        int x = toggleButtonList.get(saveButton).getWidth();
        int zy = scaledIcons[TotalNumImages - 1].getIconHeight();
        int zx = scaledIcons[TotalNumImages - 1].getIconWidth();
        int right_x = x - zx;
        int down_y = y - zy;
        
        switch(road[saveButton]){
            case 0:
                deleteLayer();
                break;
            case 1:
                switch(rotate[saveButton]){
                    case 0:
                        toggleButtonList.get(saveButton).setOverlayPosition(0,0);
                        break;
                    case 1:
                        toggleButtonList.get(saveButton).setOverlayPosition(right_x,0);
                        break;
                    case 2:
                        toggleButtonList.get(saveButton).setOverlayPosition(right_x,down_y);
                        break;
                    case 3:
                        toggleButtonList.get(saveButton).setOverlayPosition(0,down_y);
                        break;
                }
                break;
            case 2:
            case 3:
                switch(rotate[saveButton]){
                    case 0:
                        toggleButtonList.get(saveButton).setOverlayPosition(right_x,down_y);
                        break;
                    case 1:
                        toggleButtonList.get(saveButton).setOverlayPosition(0,down_y);
                        break;
                    case 2:
                        toggleButtonList.get(saveButton).setOverlayPosition(0,0);
                        break;
                    case 3:
                        toggleButtonList.get(saveButton).setOverlayPosition(right_x,0);
                        break;
                }
                break;
            case 4:
                switch(rotate[saveButton]){
                    case 0:
                        toggleButtonList.get(saveButton).setOverlayPosition(right_x,down_y);
                        break;
                    case 1:
                        toggleButtonList.get(saveButton).setOverlayPosition(0,down_y);
                        break;
                    case 2:
                        toggleButtonList.get(saveButton).setOverlayPosition(0,0);
                        break;
                    case 3:
                        toggleButtonList.get(saveButton).setOverlayPosition(right_x,0);
                        break;
                }
                break;
            case 5:
                deleteLayer();
                break;
        }
    
    }
    
    private void deleteLayer(){
        setOverlayTrue[saveButton] = false;
        toggleButtonList.get(saveButton).setOverlayIcon(null);   
    }
    
class OverlayButton extends JButton {
    private ImageIcon baseIcon;
    private ImageIcon overlayIcon;
    private int overlayX; // X-coordinate for the overlay icon
    private int overlayY; // Y-coordinate for the overlay icon

    public OverlayButton(ImageIcon baseIcon, ImageIcon overlayIcon) {
        this.baseIcon = baseIcon;
        this.overlayIcon = overlayIcon;
        this.overlayX = 0;
        this.overlayY = 0;
    }

    public void setOverlayIcon(ImageIcon overlayIcon) {
        this.overlayIcon = overlayIcon;
        repaint(); // Repaint the button to reflect the changes
    }
        
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        // Paint the base icon
        if (baseIcon != null) {
            baseIcon.paintIcon(this, g, 0, 0);
        }
        // Paint the overlay icon at the specified position
        if (overlayIcon != null) {
            overlayIcon.paintIcon(this, g, overlayX, overlayY);
        }
    }

    // Method to set the position of the overlay icon
    public void setOverlayPosition(int x, int y) {
        this.overlayX = x;
        this.overlayY = y;
        repaint(); // Repaint the button to update the display
    }

    public Icon getOverlayIcon() {
        return overlayIcon;
    }
}
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jPanel1 = new javax.swing.JPanel();
        jPanel2 = new javax.swing.JPanel();
        jLabel1 = new javax.swing.JLabel();
        jTextField1 = new javax.swing.JTextField();
        jTextField2 = new javax.swing.JTextField();
        jLabel2 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jLabelStraightRoad = new javax.swing.JLabel();
        jLabel9 = new javax.swing.JLabel();
        jLabel10 = new javax.swing.JLabel();
        jLabel11 = new javax.swing.JLabel();
        jButton1 = new javax.swing.JButton();
        jButton2 = new javax.swing.JButton();
        jButton3 = new javax.swing.JButton();
        jButton5 = new javax.swing.JButton();
        jButton6 = new javax.swing.JButton();
        jButton7 = new javax.swing.JButton();
        jSeparator1 = new javax.swing.JSeparator();
        jScrollPane2 = new javax.swing.JScrollPane();
        ResultText = new javax.swing.JTextArea();
        jButton4 = new javax.swing.JButton();
        jButton8 = new javax.swing.JButton();
        jButton9 = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        jPanel1.setBorder(javax.swing.BorderFactory.createCompoundBorder());

        jPanel2.setBorder(javax.swing.BorderFactory.createEtchedBorder());

        jLabel1.setText("Size");
        jLabel1.setAutoscrolls(true);

        jTextField1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jTextField1ActionPerformed(evt);
            }
        });

        jTextField2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jTextField2ActionPerformed(evt);
            }
        });

        jLabel2.setText("colomn");

        jLabel3.setText("row");

        jLabelStraightRoad.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/straight-road.png"))); // NOI18N
        jLabelStraightRoad.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseReleased(java.awt.event.MouseEvent evt) {
                jLabelStraightRoadMouseReleased(evt);
            }
        });

        jLabel9.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/3-way-intersection.png"))); // NOI18N
        jLabel9.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseReleased(java.awt.event.MouseEvent evt) {
                jLabel3WayIntersectionMouseReleased(evt);
            }
        });

        jLabel10.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/curve-road.png"))); // NOI18N
        jLabel10.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseReleased(java.awt.event.MouseEvent evt) {
                jLabelCurveRoadMouseReleased(evt);
            }
        });

        jLabel11.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/4-way-intersection.png"))); // NOI18N
        jLabel11.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseReleased(java.awt.event.MouseEvent evt) {
                jLabel4WayIntersectionMouseReleased(evt);
            }
        });

        jButton1.setMnemonic('r');
        jButton1.setText("rotate road");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRotateActionPerformed(evt);
            }
        });

        jButton2.setText("mirror 3-way");
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt);
            }
        });

        jButton3.setMnemonic('d');
        jButton3.setText("delete road");
        jButton3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jDeleteActionPerformed(evt);
            }
        });

        jButton5.setText("scale up");
        jButton5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ScaleUpButton(evt);
            }
        });

        jButton6.setText("scale down");
        jButton6.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ScaleDownButton(evt);
            }
        });

        jButton7.setText("Input \nMap Size");
        jButton7.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jBInputMapSize(evt);
            }
        });

        ResultText.setEditable(false);
        ResultText.setColumns(20);
        ResultText.setRows(5);
        jScrollPane2.setViewportView(ResultText);

        jButton4.setText("DRC");
        jButton4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                DRC(evt);
            }
        });

        jButton8.setMnemonic('A');
        jButton8.setText("Add destination");
        jButton8.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton8ActionPerformed(evt);
            }
        });

        jButton9.setMnemonic('e');
        jButton9.setText("Delete destination");
        jButton9.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton9ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jSeparator1)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGap(50, 50, 50)
                        .addComponent(jLabel1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.CENTER)
                            .addComponent(jButton7, javax.swing.GroupLayout.PREFERRED_SIZE, 108, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGroup(jPanel2Layout.createSequentialGroup()
                                .addComponent(jTextField1, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(14, 14, 14)
                                .addComponent(jTextField2, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(jPanel2Layout.createSequentialGroup()
                                .addComponent(jLabel2)
                                .addGap(40, 40, 40)
                                .addComponent(jLabel3))))
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(jButton8)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jButton9)))
                .addContainerGap(12, Short.MAX_VALUE))
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                .addGap(27, 27, 27)
                .addComponent(jLabelStraightRoad)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(jLabel10)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(0, 0, Short.MAX_VALUE)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 226, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGap(18, 18, 18)
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jButton2)
                            .addGroup(jPanel2Layout.createSequentialGroup()
                                .addGap(6, 6, 6)
                                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(jButton6)
                                    .addComponent(jButton1))))
                        .addGap(21, 21, 21)
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jButton4)
                            .addComponent(jButton5)
                            .addComponent(jButton3)))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                        .addComponent(jLabel9)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jLabel11)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel2)
                    .addComponent(jLabel3))
                .addGap(5, 5, 5)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel1)
                    .addComponent(jTextField1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jTextField2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jButton7, javax.swing.GroupLayout.PREFERRED_SIZE, 31, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(12, 12, 12)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jButton8)
                    .addComponent(jButton9))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(jSeparator1, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabelStraightRoad)
                    .addComponent(jLabel10))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jLabel9)
                    .addComponent(jLabel11))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jButton2)
                    .addComponent(jButton3))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jButton5)
                    .addComponent(jButton6))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jButton1)
                    .addComponent(jButton4))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 318, Short.MAX_VALUE)
                .addGap(15, 15, 15))
        );

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 800, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(jScrollPane1)
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void jButton9ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton9ActionPerformed
        deleteLayer();
    }//GEN-LAST:event_jButton9ActionPerformed

    private void jButton8ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton8ActionPerformed
        setOverlayTrue[saveButton] = true;
        imageLayer();
    }//GEN-LAST:event_jButton8ActionPerformed

    private void DRC(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_DRC
        designRuleCheck();
    }//GEN-LAST:event_DRC

    private void jBInputMapSize(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jBInputMapSize
        ButtonInputMapSize();

    }//GEN-LAST:event_jBInputMapSize

    private void ScaleDownButton(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ScaleDownButton
        scaleDown();
        setIcon();
    }//GEN-LAST:event_ScaleDownButton

    private void ScaleUpButton(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ScaleUpButton
        scaleUp();
        setIcon();
    }//GEN-LAST:event_ScaleUpButton

    private void jDeleteActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jDeleteActionPerformed
        ButtonDelete(saveButton);
    }//GEN-LAST:event_jDeleteActionPerformed

    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton2ActionPerformed
        ButtonMirror();
    }//GEN-LAST:event_jButton2ActionPerformed

    private void jRotateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRotateActionPerformed
        ButtonRotate();
    }//GEN-LAST:event_jRotateActionPerformed

    //sets the currently selected tile to a 4-way-intersection
    private void jLabel4WayIntersectionMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jLabel4WayIntersectionMouseReleased
        FourWayIntersectionSelect();
    }//GEN-LAST:event_jLabel4WayIntersectionMouseReleased

    //sets the currently selected tile to a curved road
    private void jLabelCurveRoadMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jLabelCurveRoadMouseReleased
        CurveRoadSelect();
    }//GEN-LAST:event_jLabelCurveRoadMouseReleased

    private void jLabel3WayIntersectionMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jLabel3WayIntersectionMouseReleased
        ThreeWayIntersectionSelect();
    }//GEN-LAST:event_jLabel3WayIntersectionMouseReleased

    //sets the currently selected tile to a straight road
    private void jLabelStraightRoadMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jLabelStraightRoadMouseReleased
        StraightRoadSelect();
    }//GEN-LAST:event_jLabelStraightRoadMouseReleased

    private void jTextField2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jTextField2ActionPerformed
        jTextField2.requestFocusInWindow();
    }//GEN-LAST:event_jTextField2ActionPerformed

    private void jTextField1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jTextField1ActionPerformed
        jTextField1.requestFocusInWindow();
    }//GEN-LAST:event_jTextField1ActionPerformed



    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(NewJFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(NewJFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(NewJFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(NewJFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                NewJFrame frame = new NewJFrame();
                frame.setVisible(true);
                frame.jPanel1.requestFocusInWindow(); // Set focus to jPanel1
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JTextArea ResultText;
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton3;
    private javax.swing.JButton jButton4;
    private javax.swing.JButton jButton5;
    private javax.swing.JButton jButton6;
    private javax.swing.JButton jButton7;
    private javax.swing.JButton jButton8;
    private javax.swing.JButton jButton9;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel10;
    private javax.swing.JLabel jLabel11;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel9;
    private javax.swing.JLabel jLabelStraightRoad;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JSeparator jSeparator1;
    private javax.swing.JTextField jTextField1;
    private javax.swing.JTextField jTextField2;
    // End of variables declaration//GEN-END:variables
}

