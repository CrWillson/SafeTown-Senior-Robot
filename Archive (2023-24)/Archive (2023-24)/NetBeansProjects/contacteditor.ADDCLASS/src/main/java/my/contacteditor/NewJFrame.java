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
import my.contacteditor.Tile;
import my.contacteditor.IconBorder;




/**
 *
 * @author tojo9
 */
  // Tile class to represent each tile in the town matrix



public class NewJFrame extends javax.swing.JFrame {
    
    //this variable saves which button is currently selected 
    private int saveButton;
    // Declare and initialize an ArrayList to hold JToggleButtons
    private ArrayList<JToggleButton> toggleButtonList = new ArrayList<>(); 
    
    
    private int colSize = 1;
    private int rowSize = 1;
    private int initialSize = 100;
    private int scaleBy = 10;
    private int OriginalImages = 6;
    private int TotalNumImages = OriginalImages * 4;
    JPanel panel = new JPanel();
    ImageIcon[] scaledIcons = new ImageIcon[TotalNumImages];
    
     private ArrayList<Tile> tiles = new ArrayList<>(); // List to hold tile objects
     
    // Other code...

     public NewJFrame() {
        initComponents();
//        setupKeyboardShortcuts();
        jScrollPane1.setViewportView(panel);
    } 
     
    private void JBInputMapSizeAction() {
        scaleBy = -5;
        if(jScrollPane1.getWidth()/colSize > jScrollPane1.getHeight()/rowSize){
            initialSize = jScrollPane1.getWidth()/colSize;
        }
        else{
            initialSize = jScrollPane1.getHeight()/rowSize;
        }
        
        panel.setLayout(new GridLayout(rowSize, colSize));

        // Clear the existing grid of buttons
        panel.removeAll();
        toggleButtonList.clear();
        for(int i = 0; i < rowSize*colSize; i++){
            createTilesAndButtons();
        }
        
        setIcon();  
        
    }
     
    
    // Method to create tiles and corresponding buttons
    private void createTilesAndButtons() {
        // Create tiles and add them to the list
        for (int i = 0; i < rowSize * colSize; i++) {
            Tile tile = new Tile(0, 0, false); // Example: Initialize with default values
            // Set initial properties for the tile
            tiles.add(tile);
            // Create corresponding button with appropriate icon
            JToggleButton button = createButtonForTile(tile);
            panel.add(button);
            toggleButtonList.add(button);
        }
    }
    
    private JToggleButton createButtonForTile(Tile tile) {
        JToggleButton button = new JToggleButton();
        button.setSize(new Dimension(initialSize, initialSize));
        button.setBorder(null);
        // Add the panel to the JScrollPane      
  
        final int buttonIndex = toggleButtonList.size();  
        button.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/Button-Background.png")));
        button.setIcon(getIconForTile(tile));
        tile.setRoadType(5);
        
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                saveButton = tiles.size() - 2;
                if (button.isSelected()) {
                    ButtonToggle();                   
                    // The button was toggled on
                    System.out.println("Button " + buttonIndex + " was toggled ON.");
                } else {
                    // The button was toggled off
                    System.out.println("Button " + buttonIndex + " was toggled OFF.");
                }
        }});
        
        // Set initial icon based on tile properties
        
        return button;  
    }

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
    
    private void setIcon(){
                   
            updateIconSize();

            // Force layout update to reflect changes
            panel.revalidate();
            panel.repaint();
        
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
                
                // Update icons for components
            for(int i = 0; i < colSize*rowSize; i++){
                if(toggleButtonList.get(i).getIcon() != null){
                    toggleButtonList.get(i).setIcon(scaledIcons[tiles.get(i).getRoadType()*4 + tiles.get(i).getRotation()]);
                }
            }
            
        } catch (IOException e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(this, "Error loading image: " + e.getMessage(), "Image Loading Error", JOptionPane.ERROR_MESSAGE);
        }
        
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
        }
        
        //returns the file path to the image
        return(icon);
    }
    
    
    
    // Method to get icon for a tile based on its properties
    private ImageIcon getIconForTile(Tile tile) {
        // Implement logic to select appropriate icon based on tile properties
        // You can use a switch statement or if-else conditions
        // Example:
        String iconPath = determineIconPath(tile);
        return new ImageIcon(getClass().getResource(iconPath));
    }

    // Method to determine icon path based on tile properties
    private String determineIconPath(Tile tile) {
        // Implement logic to determine icon path based on tile properties
        // Example:
        switch (tile.getRoadType()) {
            case 0: // Straight road
                return "/images/straight-road.png";
            case 1: // Curved road
                return "/images/curve-road.png";
            // Other cases for different road types...
            default:
                return "/images/Button-Background.png"; // Default icon
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
        jScrollPane1 = new javax.swing.JScrollPane();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        jPanel1.setBorder(javax.swing.BorderFactory.createCompoundBorder());

        jPanel2.setBorder(javax.swing.BorderFactory.createEtchedBorder());

        jLabel1.setText("Size");
        jLabel1.setAutoscrolls(true);

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

        jButton8.setText("jButton8");
        jButton8.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton8ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jSeparator1)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                .addGap(0, 0, Short.MAX_VALUE)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addComponent(jLabel9)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel11))
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addComponent(jLabelStraightRoad)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel10))
                    .addGroup(jPanel2Layout.createSequentialGroup()
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
                            .addComponent(jButton3))))
                .addGap(17, 17, 17))
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGap(50, 50, 50)
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addGroup(jPanel2Layout.createSequentialGroup()
                                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                    .addComponent(jLabel2)
                                    .addGroup(jPanel2Layout.createSequentialGroup()
                                        .addComponent(jLabel1)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jTextField1, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                .addGap(14, 14, 14)
                                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(jTextField2, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addGroup(jPanel2Layout.createSequentialGroup()
                                        .addGap(26, 26, 26)
                                        .addComponent(jLabel3))))
                            .addComponent(jButton7, javax.swing.GroupLayout.PREFERRED_SIZE, 108, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGap(84, 84, 84)
                        .addComponent(jButton8)))
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
                .addComponent(jButton8)
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
                .addGap(21, 21, 21)
                .addComponent(jScrollPane2)
                .addContainerGap())
        );

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 800, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 800, Short.MAX_VALUE)
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

    //sets the currently selected tile to a 4-way-intersection
    private void jLabel4WayIntersectionMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jLabel4WayIntersectionMouseReleased
//        FourWayIntersectionSelect();
    }//GEN-LAST:event_jLabel4WayIntersectionMouseReleased

    //sets the currently selected tile to a curved road
    private void jLabelCurveRoadMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jLabelCurveRoadMouseReleased
 //       CurveRoadSelect();
    }//GEN-LAST:event_jLabelCurveRoadMouseReleased

    //sets the currently selected tile to a straight road
    private void jLabelStraightRoadMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jLabelStraightRoadMouseReleased
//        StraightRoadSelect();
    }//GEN-LAST:event_jLabelStraightRoadMouseReleased

    private void jLabel3WayIntersectionMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jLabel3WayIntersectionMouseReleased
//        ThreeWayIntersectionSelect();
    }//GEN-LAST:event_jLabel3WayIntersectionMouseReleased

    private void ScaleUpButton(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ScaleUpButton
//        scaleUp();
//        setIcon();
    }//GEN-LAST:event_ScaleUpButton

    private void ScaleDownButton(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ScaleDownButton
//        scaleDown();
//        setIcon();
    }//GEN-LAST:event_ScaleDownButton

    private void jBInputMapSize(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jBInputMapSize
        try {
                colSize = Integer.parseInt(jTextField1.getText()); // Try to parse the text as an integer
                rowSize = Integer.parseInt(jTextField2.getText());
            } catch (NumberFormatException ex) {
                // If parsing fails, show a warning message

            }
        JBInputMapSizeAction();
        
    }//GEN-LAST:event_jBInputMapSize

    private void jRotateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRotateActionPerformed
//        ButtonRotate();
    }//GEN-LAST:event_jRotateActionPerformed

    private void jDeleteActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jDeleteActionPerformed
//        ButtonDelete(saveButton);
    }//GEN-LAST:event_jDeleteActionPerformed

    private void DRC(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_DRC
//        designRuleCheck();
    }//GEN-LAST:event_DRC

    private void jButton8ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton8ActionPerformed

    }//GEN-LAST:event_jButton8ActionPerformed

    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton2ActionPerformed
//        ButtonMirror();
    }//GEN-LAST:event_jButton2ActionPerformed

       



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

// Class for overlaying icons
class OverlayIcon implements Icon {
    private final Icon baseIcon;
    private final Icon overlayIcon;
    private int overlayX; // X-coordinate for the overlay icon
    private int overlayY; // Y-coordinate for the overlay icon

    public OverlayIcon(Icon baseIcon, Icon overlayIcon, int overlayX, int overlayY) {
        this.baseIcon = baseIcon;
        this.overlayIcon = overlayIcon;
        this.overlayX = overlayX;
        this.overlayY = overlayY;
    }

     @Override
    public void paintIcon(Component c, Graphics g, int x, int y) {
        baseIcon.paintIcon(c, g, x, y);
        // Paint the overlay icon at the specified position relative to the base icon
        overlayIcon.paintIcon(c, g, x + overlayX, y + overlayY);
    }

    @Override
    public int getIconWidth() {
        int baseWidth = baseIcon.getIconWidth();
        int overlayWidth = overlayIcon.getIconWidth();
        return Math.max(baseWidth, overlayWidth) + Math.abs(overlayX); // Adjusted width considering overlay position
    }

    @Override
    public int getIconHeight() {
        int baseHeight = baseIcon.getIconHeight();
        int overlayHeight = overlayIcon.getIconHeight();
        return Math.max(baseHeight, overlayHeight) + Math.abs(overlayY); // Adjusted height considering overlay position
    }
}
}

