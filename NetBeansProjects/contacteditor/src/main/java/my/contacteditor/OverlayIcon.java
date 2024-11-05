/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package my.contacteditor;

import java.awt.Component;
import java.awt.Graphics;
import javax.swing.Icon;

/**
 *
 * @author sethcampbell150
 */
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
