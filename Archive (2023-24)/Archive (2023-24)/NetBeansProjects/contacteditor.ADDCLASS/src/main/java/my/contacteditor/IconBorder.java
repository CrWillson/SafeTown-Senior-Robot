/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package my.contacteditor;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Graphics2D;
import javax.swing.AbstractButton;
import javax.swing.Icon;
import javax.swing.border.AbstractBorder;

/**
 *
 * @author sethcampbell150
 */
//creates a border around the selected image
    class IconBorder extends AbstractBorder {
        private Color borderColor;

        public IconBorder(Color borderColor) {
            this.borderColor = borderColor;
        }

        @Override
        public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
            // Draw the icon
            if (c instanceof AbstractButton) {
                Icon icon = ((AbstractButton) c).getIcon();
                if (icon != null) {
                    int iconWidth = icon.getIconWidth();
                    int iconHeight = icon.getIconHeight();
                    int iconX = x + (width - iconWidth) / 2;
                    int iconY = y + (height - iconHeight) / 2;
                    icon.paintIcon(c, g, iconX, iconY);
                }
            }

            // Draw the border around the icon
            Graphics2D g2d = (Graphics2D) g;
            g2d.setColor(borderColor);
            g2d.setStroke(new BasicStroke(2)); // Adjust border thickness as needed
            g2d.drawRect(x, y, width - 1, height - 1); // Adjust border position and size as needed
        }

    }
