#pragma once

#include <vector>
#include <Constants.h>
#include <stdint.h>

namespace utils
{
    /// @brief This function rotates a Matrix by 90 degrees clockwise or counter-clockwise.
    /// @param matrix Matrix to rotate
    /// @param clockWise clockwise or counterclockwise (order of rotation)
    /// @return Return a rotated matrix (usually by 90 degrees clockwise or c-clockwise)
    [[nodiscard]] inline auto RotateMatrix(const std::vector<uint8_t>& matrix, bool clockWise = true) -> std::vector<uint8_t>
    {
        // nota: le matrici vengono ruotate di 90 gradi scambiando righe e colonne, qui sotto spiego brevemente l'algoritmo che ho
        // utilizzato per fare cio'.

        // Creo un vettore temporaneo per potermi salvare la nuova matrice una volta che e' stata trasposta
        std::vector<uint8_t> rotatedMatrix(matrix.size());

        // Qui ci sono vari indici tra i quali:
        // columnIndex: mi serve per sapere quale colonna sto iterando

        auto columnIndex = 0;
        auto index = 0;

        const auto matrixRowLength = static_cast<int>(std::sqrt(static_cast<int>(matrix.size())));

        auto startRowIndex = clockWise ? matrixRowLength - 1 : 0;
        auto endRowIndex = matrixRowLength - 1;

        for (auto i = 0ul; i != matrix.size(); ++i)
        {
            // Se il resto della divisione ha dato zero significa che ho terminato la prima riga quindi passo alla seconda colonna
            if ((i % matrixRowLength == 0) && (i > 0))
            {
                columnIndex++;
                index = 0;
                startRowIndex = clockWise ? matrixRowLength - 1 : 0;
                endRowIndex--;
            }

            /*
                Esempio della formula in ordine CLOCKWISE, ruotando di 90 gradi:

                (iterazione 0) -> index = 0, matrixRowLength = 4, columnIndex = 0 => 0 * 4 + 0 = 0
                (iterazione 1) -> index = 1, matrixRowLength = 4, columnIndex = 0 => 1 * 4 + 0 = 4
                (iterazione 2) -> index = 2, matrixRowLength = 4, columnIndex = 0 => 2 * 4 + 0 = 8
                (iterazione 2) -> index = 2, matrixRowLength = 4, columnIndex = 0 => 3 * 4 + 0 = 12

                in questo modo ho preso la prima colonna scorrendo dall'alto verso il basso, visto che l'array e' 
                monodimensionale ovvero spiattellato lo vedremmo tipo: 0,0,0,1, 0,0,0,1, 0,0,0,1, 0,0,0,1
                allora cosi facendo si và a prendere ogni elemento che sta all'inizio di ogni gruppo (per le colonne).
            */

            const auto matrixIndex = clockWise 
                    ? index * matrixRowLength + columnIndex 
                    : index * matrixRowLength + endRowIndex;

            // dopo aver calcolato l'indice giusto all'interno della matrice, vado a prendermi il valore che la matrice ha a quell'indice

            const auto valueToTranspose = matrix[matrixIndex];

            // qui semplicemente copio i valori della matrice trasposta in quella nuova.

            const auto transposedMatrixIndex = columnIndex * matrixRowLength + startRowIndex;
            
            rotatedMatrix[transposedMatrixIndex] = valueToTranspose;

            index++;
            startRowIndex += clockWise ? -1 : 1;
        }

        return rotatedMatrix;
    }

    [[nodiscard]] inline static auto Lerp(float a, float b, float t) -> float
    {
        // Assicurati che il parametro "t" sia compreso tra 0 e 1
        t = std::clamp(t, 0.0f, 1.0f);

        // Calcola il risultato dell'interpolaizone lineare
        return a + t * (b - a);
    }
}